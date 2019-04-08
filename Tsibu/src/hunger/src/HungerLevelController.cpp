#include "../include/HungerLevelController.hpp"

HungerLevelController::HungerLevelController(FSM<HungerLevel>* f, FSMSystemCommunicator* fsc)
	: FSMController(HUNGER_LEVEL_FSM_NAME, f, HUNGER_LEVEL_REEVALUATION_RATE_MILLISECONDS, fsc) {}

void HungerLevelController::init()
{
	fsm->set_current_state(new HungerLevel(HungerLevel::FULL));

	fsm->set_variable<std::chrono::system_clock::time_point>(
	  HUNGER_LEVEL_VARIABLE_LAST_STATE_INCREMENT,
	  new std::chrono::system_clock::time_point(std::chrono::system_clock::now())
	);

	fsm->set_variable<double>(
	  HUNGER_LEVEL_VARIABLE_CUMULATIVE_SATIATION,
	  new double(0.0)
	);
}

void HungerLevelController::update_inputs()
{
	// No inputs
}

bool HungerLevelController::process()
{
	// Get the current state as its int representation
	int current_state_int = static_cast<int>(*(fsm->get_current_state()));
	int next_state_int = current_state_int;

	// Get current state of the FSM for hunger change magnitude as its int representation
	int current_hunger_change_magnitude_int = static_cast<int>(*(dynamic_cast<FSM<HungerChangeMagnitude>*>(
	  fsm_system_communicator->get_base_FSM(HUNGER_CHANGE_MAGNITUDE_FSM_NAME)
	)->get_current_state()));

	// Multiply by a normalized factor
	double normalized_hunger_change_magnitude = static_cast<double>(
	  (current_hunger_change_magnitude_int - static_cast<int>(HungerChangeMagnitude::NO_SATISFACTION)) * HUNGER_LEVEL_CHANGE_MAGNITUDE_FACTOR
	);

	// Add the contribution to the cumulative satiation
	double* new_cumulative_satiation = new double(
	  dynamic_cast<FSMVariable<double>*>(fsm->get_variable(HUNGER_LEVEL_VARIABLE_CUMULATIVE_SATIATION))->get() + normalized_hunger_change_magnitude
	);

	// If enough hunger is satiated, make the AI less hungry
	if ((*new_cumulative_satiation) > 0)
	{
	  int int_contribution = static_cast<int>(*new_cumulative_satiation);
	  next_state_int -= int_contribution;
	  (*new_cumulative_satiation) -= int_contribution;
	}

	// Set the new satiation
	fsm->set_variable<double>(HUNGER_LEVEL_VARIABLE_CUMULATIVE_SATIATION, new_cumulative_satiation);

	// If enough time as passed, make the AI hungrier
	std::chrono::system_clock::time_point time_now = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(
	  time_now - dynamic_cast<FSMVariable<std::chrono::system_clock::time_point>*>(fsm->get_variable(HUNGER_LEVEL_VARIABLE_LAST_STATE_INCREMENT))->get()
	).count() < HUNGER_LEVEL_STATE_INCREMENT_SECONDS)
	{
	  next_state_int++;
	  fsm->set_variable<std::chrono::system_clock::time_point>(
	    HUNGER_LEVEL_VARIABLE_LAST_STATE_INCREMENT,
	    new std::chrono::system_clock::time_point(time_now)
	  );
	}

	if (next_state_int < static_cast<int>(HungerLevel::FULL))
	{
	  next_state_int = static_cast<int>(HungerLevel::FULL);
	}
	else if (next_state_int > static_cast<int>(HungerLevel::FAMISHED))
	{
	  next_state_int = static_cast<int>(HungerLevel::FAMISHED);
	}

	fsm->set_current_state(new HungerLevel(HungerLevel(next_state_int)));
	return next_state_int != current_state_int;
}
