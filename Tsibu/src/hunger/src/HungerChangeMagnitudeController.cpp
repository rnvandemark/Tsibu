#include "../include/HungerChangeMagnitudeController.hpp"

void HungerChangeMagnitudeController::init()
{
	fsm->set_current_state(new HungerChangeMagnitude(HungerChangeMagnitude::NO_SATISFACTION));
	
	update_inputs();
}

void HungerChangeMagnitudeController::update_inputs()
{
	fsm->set_variable<uint16_t>(
		HUNGER_CHANGE_MAGNITUDE_VARIABLE_LEFT_PHOTORESISTOR,
		new uint16_t(serial_communicator->get_value_for_descriptor<uint16_t>(HUNGER_CHANGE_MAGNITUDE_LEFT_PR_DESCRIPTOR))
	);
	
	fsm->set_variable<uint16_t>(
		HUNGER_CHANGE_MAGNITUDE_VARIABLE_CENTER_PHOTORESISTOR,
		new uint16_t(serial_communicator->get_value_for_descriptor<uint16_t>(HUNGER_CHANGE_MAGNITUDE_CENTER_PR_DESCRIPTOR))
	);
	
	fsm->set_variable<uint16_t>(
		HUNGER_CHANGE_MAGNITUDE_VARIABLE_RIGHT_PHOTORESISTOR,
		new uint16_t(serial_communicator->get_value_for_descriptor<uint16_t>(HUNGER_CHANGE_MAGNITUDE_RIGHT_PR_DESCRIPTOR))
	);
	
	fsm->set_variable<uint16_t>(
		HUNGER_CHANGE_MAGNITUDE_VARIABLE_REAR_PHOTORESISTOR,
		new uint16_t(serial_communicator->get_value_for_descriptor<uint16_t>(HUNGER_CHANGE_MAGNITUDE_REAR_PR_DESCRIPTOR))
	);
	
	std::cout << "LT: " << static_cast<int>(dynamic_cast<FSMVariable<uint16_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_LEFT_PHOTORESISTOR))->get()) << std::endl;
	std::cout << "CR: " << static_cast<int>(dynamic_cast<FSMVariable<uint16_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_CENTER_PHOTORESISTOR))->get()) << std::endl;
	std::cout << "RT: " << static_cast<int>(dynamic_cast<FSMVariable<uint16_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_RIGHT_PHOTORESISTOR))->get()) << std::endl;
	std::cout << "RR: " << static_cast<int>(dynamic_cast<FSMVariable<uint16_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_REAR_PHOTORESISTOR))->get()) << std::endl;
}

bool HungerChangeMagnitudeController::process()
{
	int current_state_int = static_cast<int>(*(fsm->get_current_state()));

	uint16_t average = 0;
	average += dynamic_cast<FSMVariable<uint16_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_LEFT_PHOTORESISTOR))->get();
	average += dynamic_cast<FSMVariable<uint16_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_CENTER_PHOTORESISTOR))->get();
	average += dynamic_cast<FSMVariable<uint16_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_RIGHT_PHOTORESISTOR))->get();
	average += dynamic_cast<FSMVariable<uint16_t>*>(fsm->get_variable(HUNGER_CHANGE_MAGNITUDE_VARIABLE_REAR_PHOTORESISTOR))->get();
	average /= 4;
	
	int next_state_int = 0;
	if (average < HUNGER_CHANGE_MAGNITUDE_LITTLE_SATISFACTION_THRESHOLD)
	{
		next_state_int = static_cast<int>(HungerChangeMagnitude::NO_SATISFACTION);
	}
	else if (average < HUNGER_CHANGE_MAGNITUDE_GOOD_SATISFACTION_THRESHOLD)
	{
		next_state_int = static_cast<int>(HungerChangeMagnitude::LITTLE_SATISFACTION);
	}
	else if (average < HUNGER_CHANGE_MAGNITUDE_EXTREME_SATISFACTION_THRESHOLD)
	{
		next_state_int = static_cast<int>(HungerChangeMagnitude::GOOD_SATISFACTION);
	}
	else
	{
		next_state_int = static_cast<int>(HungerChangeMagnitude::EXTREME_SATISFACTION);
	}
	
	fsm->set_current_state(new HungerChangeMagnitude(HungerChangeMagnitude(next_state_int)));
	return current_state_int != next_state_int;
}

HungerChangeMagnitudeController::HungerChangeMagnitudeController(FSM<HungerChangeMagnitude>* f, FSMSystemCommunicator* fsc, SerialCommunicator* sc)
: FSMController(HUNGER_CHANGE_MAGNITUDE_FSM_NAME, f, HUNGER_CHANGE_MAGNITUDE_REEVALUATION_RATE_MILLISECONDS, fsc)
{
	serial_communicator = sc;
}

HungerChangeMagnitudeController::~HungerChangeMagnitudeController()
{
	// Do nothing
}