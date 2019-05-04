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

	hunger_level_fsm_controller.start_routine();
	hunger_change_magnitude_fsm_controller.start_routine();

	std::cout << "[TsibuMain] Started all FSMs." << std::endl;

	while (keep_alive)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	hunger_level_fsm_controller.stop_routine_and_join();
	hunger_change_magnitude_fsm_controller.stop_routine_and_join();

	delete serial_communicator;
	delete FSM_communicator;

	std::cout << "[TsibuMain] Cleanly exiting." << std::endl;
	return 0;
}