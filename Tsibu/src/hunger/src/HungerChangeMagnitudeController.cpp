#include "../include/HungerChangeMagnitudeController.hpp"

void HungerChangeMagnitudeController::init()
{
	fsm->set_current_state(new HungerChangeMagnitude(HungerChangeMagnitude::NO_SATISFACTION));
	
	update_inputs();
}

void HungerChangeMagnitudeController::update_inputs()
{
	// Get photoresistor values from static helper here
	// Generate a random number for now
	
	int16_t pr_current_value_left = left_photoresistor_gpio->get_current_analog_value();
	if (pr_current_value_left < 0)
	{
		std::cout << "ERROR reading left photoresistor value." << std::endl;
	}
	else
	{
		if (pr_current_value_left >= HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE)
		{
			pr_current_value_left = HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE - 1;
		}
		
		fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_LEFT_PHOTORESISTOR, new uint8_t(static_cast<uint8_t>(pr_current_value_left)));
	}
	
	int16_t pr_current_value_middle = middle_photoresistor_gpio->get_current_analog_value();
	if (pr_current_value_middle < 0)
	{
		std::cout << "ERROR reading middle photoresistor value." << std::endl;
	}
	else
	{
		if (pr_current_value_middle >= HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE)
		{
			pr_current_value_middle = HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE - 1;
		}
		
		fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_MIDDLE_PHOTORESISTOR, new uint8_t(static_cast<uint8_t>(pr_current_value_middle)));
	}
	
	int16_t pr_current_value_right = right_photoresistor_gpio->get_current_analog_value();
	if (pr_current_value_right < 0)
	{
		std::cout << "ERROR reading right photoresistor value." << std::endl;
	}
	else
	{
		if (pr_current_value_right >= HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE)
		{
			pr_current_value_right = HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE - 1;
		}
		
		fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_RIGHT_PHOTORESISTOR, new uint8_t(static_cast<uint8_t>(pr_current_value_right)));
	}
	
	int16_t pr_current_value_rear = rear_photoresistor_gpio->get_current_analog_value();
	if (pr_current_value_rear < 0)
	{
		std::cout << "ERROR reading rear photoresistor value." << std::endl;
	}
	else
	{
		if (pr_current_value_rear >= HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE)
		{
			pr_current_value_rear = HUNGER_CHANGE_MAGNITUDE_MAXIMUM_PHOTORESISTOR_VALUE - 1;
		}
		
		fsm->set_variable<uint8_t>(HUNGER_CHANGE_MAGNITUDE_VARIABLE_REAR_PHOTORESISTOR, new uint8_t(static_cast<uint8_t>(pr_current_value_rear)));
	}
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

HungerChangeMagnitudeController::HungerChangeMagnitudeController(FSM<HungerChangeMagnitude>* f, FSMSystemCommunicator* fsc)
	: FSMController(HUNGER_CHANGE_MAGNITUDE_FSM_NAME, f, HUNGER_CHANGE_MAGNITUDE_REEVALUATION_RATE_MILLISECONDS, fsc)
{
	left_photoresistor_gpio = new GPIO(HUNGER_CHANGE_MAGNITUDE_VARIABLE_LEFT_PHOTORESISTOR_PIN_INDEX, GPIOMode::INPUT);
	middle_photoresistor_gpio = new GPIO(HUNGER_CHANGE_MAGNITUDE_VARIABLE_MIDDLE_PHOTORESISTOR_PIN_INDEX, GPIOMode::INPUT);
	right_photoresistor_gpio = new GPIO(HUNGER_CHANGE_MAGNITUDE_VARIABLE_RIGHT_PHOTORESISTOR_PIN_INDEX, GPIOMode::INPUT);
	rear_photoresistor_gpio = new GPIO(HUNGER_CHANGE_MAGNITUDE_VARIABLE_REAR_PHOTORESISTOR_PIN_INDEX, GPIOMode::INPUT);
}

HungerChangeMagnitudeController::~HungerChangeMagnitudeController()
{
	delete left_photoresistor_gpio;
	delete middle_photoresistor_gpio;
	delete right_photoresistor_gpio;
	delete rear_photoresistor_gpio;
}