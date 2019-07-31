#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>

#include "../src/common/include/FSMSystemCommunicator.hpp"
#include "../src/common/include/FSM.hpp"

#include "../src/common/include/GPIORegistrar.hpp"

#include "../src/common/include/SerialCommunicator.hpp"
#include "../src/common/include/BaudRate.hpp"

#include "../src/common/include/MCP3008SPI.hpp"

#include "../src/hunger/include/HungerLevel.hpp"
#include "../src/hunger/include/HungerLevelController.hpp"

#include "../src/hunger/include/HungerChangeMagnitude.hpp"
#include "../src/hunger/include/HungerChangeMagnitudeController.hpp"

#include "../src/operation_mode/include/OperationMode.hpp"
#include "../src/operation_mode/include/CurrentOperationModeController.hpp"

#include "../src/motor_control/include/MovementDirection.hpp"
#include "../src/motor_control/include/CurrentMovementDirectionController.hpp"

#include "../src/awareness/include/SurroundingsAnalysis.hpp"
#include "../src/awareness/include/SurroundingsAnalysisController.hpp"

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
	
	{
		HungerLevelController hunger_level_fsm_controller(new FSM<HungerLevel>(), FSM_communicator);
		HungerChangeMagnitudeController hunger_change_magnitude_fsm_controller(new FSM<HungerChangeMagnitude>(), FSM_communicator, serial_communicator);
		CurrentOperationModeController current_operation_mode_fsm_controller(new FSM<OperationMode>(), FSM_communicator);
		CurrentMovementDirectionController current_movement_direction_fsm_controller(new FSM<MovementDirection>(), FSM_communicator, serial_communicator);
		SurroundingsAnalysisController surroundings_analysis_fsm_controller(new FSM<SurroundingsAnalysis>(), FSM_communicator);
		
		hunger_level_fsm_controller.start_routine();
		hunger_change_magnitude_fsm_controller.start_routine();
		current_operation_mode_fsm_controller.start_routine();
		current_movement_direction_fsm_controller.start_routine();
		surroundings_analysis_fsm_controller.start_routine();
		
		std::cout << "[TsibuMain] Started all FSMs." << std::endl;
		
		const int tick_rate = 5;
		int ticks = 0;
		while (keep_alive)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(tick_rate));
	 		ticks += tick_rate;
	 		
	 		if (ticks > 1000)
	 		{
				// Do some testing/debugging work here...
				ticks %= 1000;
			}
		}
	
		hunger_level_fsm_controller.stop_routine_and_join();
		hunger_change_magnitude_fsm_controller.stop_routine_and_join();
		current_operation_mode_fsm_controller.stop_routine_and_join();
		current_movement_direction_fsm_controller.stop_routine_and_join();
		surroundings_analysis_fsm_controller.stop_routine_and_join();
	}
	
	delete FSM_communicator;
	delete serial_communicator;
	
	std::cout << "[TsibuMain] Cleanly exiting." << std::endl;
	return 0;
}
