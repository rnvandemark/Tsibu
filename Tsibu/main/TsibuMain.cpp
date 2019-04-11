#include "../src/common/include/FSMSystemCommunicator.hpp"
#include "../src/common/include/FSM.hpp"

#include "../src/common/include/GPIORegistrar.hpp"

#include "../src/hunger/include/HungerLevel.hpp"
#include "../src/hunger/include/HungerLevelController.hpp"

#include "../src/hunger/include/HungerChangeMagnitude.hpp"
#include "../src/hunger/include/HungerChangeMagnitudeController.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>

static volatile bool keep_alive = true;

void sigint_handler(int signum)
{
	std::cout << std::endl << "Heard signal [" << signum << "], attempting to exit (please wait briefly)..." << std::endl;
	keep_alive = false;
}

int main(int argc, char** argv)
{
	signal(SIGINT, sigint_handler);
	
	for (int i = 2; i <= 27; i++)
	{
		GPIORegistrar::add_legal_pin(i);
	}

	FSMSystemCommunicator* communicator = new FSMSystemCommunicator();

	HungerLevelController hunger_level_fsm_controller(new FSM<HungerLevel>(), communicator);
	HungerChangeMagnitudeController hunger_change_magnitude_fsm_controller(new FSM<HungerChangeMagnitude>(), communicator);

	hunger_level_fsm_controller.start_routine();
	hunger_change_magnitude_fsm_controller.start_routine();

	while (keep_alive)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	hunger_level_fsm_controller.stop_routine_and_join();
	hunger_change_magnitude_fsm_controller.stop_routine_and_join();

	delete communicator;

	std::cout << "Cleanly exiting." << std::endl;
	return 0;
}
