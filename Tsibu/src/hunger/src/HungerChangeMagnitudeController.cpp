#include "../include/HungerChangeMagnitudeController.hpp"

HungerChangeMagnitudeController::HungerChangeMagnitudeController(FSM<HungerChangeMagnitude>* f, FSMSystemCommunicator* fsc)
	: FSMController(HUNGER_CHANGE_MAGNITUDE_FSM_NAME, f, HUNGER_CHANGE_MAGNITUDE_REEVALUATION_RATE_MILLISECONDS, fsc) {}

void HungerChangeMagnitudeController::init()
{
	fsm->set_current_state(new HungerChangeMagnitude(HungerChangeMagnitude::NO_SATISFACTION));

	fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_LEFT_PHOTORESISTOR, new uint8_t(0));
	fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_MIDDLE_PHOTORESISTOR, new uint8_t(0));
	fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_RIGHT_PHOTORESISTOR, new uint8_t(0));
	fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_REAR_PHOTORESISTOR, new uint8_t(0));

	srand(time(0));
}

void HungerChangeMagnitudeController::update_inputs()
{
	// Get photoresistor values from static helper here
	// Generate a random number for now

	fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_LEFT_PHOTORESISTOR, new uint8_t(rand() % HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE));
	fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_MIDDLE_PHOTORESISTOR, new uint8_t(rand() % HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE));
	fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_RIGHT_PHOTORESISTOR, new uint8_t(rand() % HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE));
	fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_REAR_PHOTORESISTOR, new uint8_t(rand() % HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE));
}

bool HungerChangeMagnitudeController::process()
{
	uint16_t average = 0;
	average += dynamic_cast<FSMVariable<uint8_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_LEFT_PHOTORESISTOR))->get();
	average += dynamic_cast<FSMVariable<uint8_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_MIDDLE_PHOTORESISTOR))->get();
	average += dynamic_cast<FSMVariable<uint8_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_RIGHT_PHOTORESISTOR))->get();
	average += dynamic_cast<FSMVariable<uint8_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_REAR_PHOTORESISTOR))->get();
	average /= 4;

	HungerChangeMagnitude* next_state = nullptr;
	if (average < HUNGER_CHANGE_MAGNITUDE_LITTLE_SATISFACTION_THRESHOLD)
	{
	  next_state = new HungerChangeMagnitude(HungerChangeMagnitude::NO_SATISFACTION);
	}
	else if (average < HUNGER_CHANGE_MAGNITUDE_GOOD_SATISFACTION_THRESHOLD)
	{
	  next_state = new HungerChangeMagnitude(HungerChangeMagnitude::LITTLE_SATISFACTION);
	}
	else if (average < HUNGER_CHANGE_MAGNITUDE_EXTREME_SATISFACTION_THRESHOLD)
	{
	  next_state = new HungerChangeMagnitude(HungerChangeMagnitude::GOOD_SATISFACTION);
	}
	else
	{
	  next_state = new HungerChangeMagnitude(HungerChangeMagnitude::EXTREME_SATISFACTION);
	}

	HungerChangeMagnitude* current_state = fsm->get_current_state();
	fsm->set_current_state(next_state);
	return *current_state != *next_state;
}
