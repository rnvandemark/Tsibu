#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>

#include "../src/common/include/FSMSystemCommunicator.hpp"
#include "../src/common/include/FSM.hpp"

#include "../src/common/include/GPIORegistrar.hpp"

#include "../src/common/include/SerialCommunicator.hpp"
#include "../src/common/include/BaudRate.hpp"

#include "../src/hunger/include/HungerLevel.hpp"
#include "../src/hunger/include/HungerLevelController.hpp"

#include "../src/hunger/include/HungerChangeMagnitude.hpp"
#include "../src/hunger/include/HungerChangeMagnitudeController.hpp"

#include "../src/operation_mode/include/OperationMode.hpp"
#include "../src/operation_mode/include/CurrentOperationModeController.hpp"

#include "../src/motor_control/include/MovementDirection.hpp"
#include "../src/motor_control/include/CurrentMovementDirectionController.hpp"

static volatile bool keep_alive = true;

void sigint_handler(int signum)
{
	std::cout << std::endl << "[TsibuMain] Heard signal [" << signum << "], attempting to exit (please wait briefly)..." << std::endl;
	keep_alive = false;
}

int main(int argc, char** argv)
{
	signal(SIGINT, sigint_handler);
	
	for (int i = 2; i <= 27; i++)
	{
		GPIORegistrar::add_legal_pin(i);
	}

	FSMSystemCommunicator* FSM_communicator = new FSMSystemCommunicator();
	
	SerialCommunicator* serial_communicator = new SerialCommunicator("/dev/ttyACM0", BaudRate::_9600, 0);

	HungerLevelController hunger_level_fsm_controller(new FSM<HungerLevel>(), FSM_communicator);
	HungerChangeMagnitudeController hunger_change_magnitude_fsm_controller(new FSM<HungerChangeMagnitude>(), FSM_communicator, serial_communicator);
	CurrentOperationModeController current_operation_mode_fsm_controller(new FSM<OperationMode>(), FSM_communicator);
	CurrentMovementDirectionController current_movement_direction_fsm_controller(new FSM<MovementDirection>(), FSM_communicator, serial_communicator);

	hunger_level_fsm_controller.start_routine();
	hunger_change_magnitude_fsm_controller.start_routine();
	current_operation_mode_fsm_controller.start_routine();
	current_movement_direction_fsm_controller.start_routine();

	std::cout << "[TsibuMain] Started all FSMs." << std::endl;
	
	int ticks = 0;
	int previous_hunger_level = 0;
	while (keep_alive)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
 		ticks += 5;
 		
 		if (ticks >= 1000)
 		{
 			// Everything below here is temporary, for testing/demonstration
 			int current_hunger_level = static_cast<int>(*(dynamic_cast<FSM<HungerLevel>*>(
				FSM_communicator->get_base_FSM(HUNGER_LEVEL_FSM_NAME)
			)->get_current_state()));
			
			if (previous_hunger_level != current_hunger_level)
			{
				bool value_LED0, value_LED1, value_LED2;
				
				switch(current_hunger_level)
				{
					case 1:
						value_LED0 = true;
						value_LED1 = false;
						value_LED2 = false;
						break;
					
					case 2:
						value_LED0 = false;
						value_LED1 = true;
						value_LED2 = false;
						break;
					
					case 3:
						value_LED0 = true;
						value_LED1 = true;
						value_LED2 = false;
						break;
					
					case 4:
						value_LED0 = false;
						value_LED1 = false;
						value_LED2 = true;
						break;
					
					case 5:
						value_LED0 = true;
						value_LED1 = false;
						value_LED2 = true;
						break;
				}
				
				if (!serial_communicator->set_digital_value_for_descriptor("DBG0", value_LED0))
				{
					std::cout << "[TsibuMain] Failed to set value for DBG0." << std::endl;
				}
				
				if (!serial_communicator->set_digital_value_for_descriptor("DBG1", value_LED1))
				{
					std::cout << "[TsibuMain] Failed to set value for DBG1." << std::endl;
				}
				
				if (!serial_communicator->set_digital_value_for_descriptor("DBG2", value_LED2))
				{
					std::cout << "[TsibuMain] Failed to set value for DBG2." << std::endl;
				}
				
				ticks = 0;
				previous_hunger_level = current_hunger_level;
			}
		}
	}
	
	if (!serial_communicator->set_digital_value_for_descriptor("DBG0", false))
	{
		std::cout << "[TsibuMain] Failed to set exit value for DBG0." << std::endl;
	}
	
	if (!serial_communicator->set_digital_value_for_descriptor("DBG1", false))
	{
		std::cout << "[TsibuMain] Failed to set exit value for DBG1." << std::endl;
	}
	
	if (!serial_communicator->set_digital_value_for_descriptor("DBG2", false))
	{
		std::cout << "[TsibuMain] Failed to set exit value for DBG2." << std::endl;
	}

	hunger_level_fsm_controller.stop_routine_and_join();
	hunger_change_magnitude_fsm_controller.stop_routine_and_join();
	current_operation_mode_fsm_controller.stop_routine_and_join();
	current_movement_direction_fsm_controller.stop_routine_and_join();

	delete serial_communicator;
	delete FSM_communicator;

	std::cout << "[TsibuMain] Cleanly exiting." << std::endl;
	return 0;
}