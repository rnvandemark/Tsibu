#include "../src/common/include/FSM.hpp"
#include "../src/hunger/include/HungerLevel.hpp"
#include "../src/hunger/include/HungerLevelController.hpp"

#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
	std::cout << "Starting program..." << std::endl;
	HungerLevelController hunger_fsm_controller("hunger", FSM<HungerLevel>(), 1000);
	std::cout << "Created hunger controller..." << std::endl;
	hunger_fsm_controller.start_routine();
	std::cout << "Started hunger routine..." << std::endl;

	std::this_thread::sleep_for(std::chrono::minutes(2));

	hunger_fsm_controller.stop_routine_and_join();

	std::cout << "Exiting program." << std::endl;

	return 0;
}
