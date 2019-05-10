#include "../include/CurrentOperationModeController.hpp"

void CurrentOperationModeController::init()
{
	fsm->set_current_state(new OperationMode(OperationMode::IDLE));
	
	update_inputs();
}

void CurrentOperationModeController::update_inputs()
{
	// Do nothing
}

bool CurrentOperationModeController::process()
{
	int current_state_int = static_cast<int>(*(fsm->get_current_state()));

	HungerLevel current_hunger_level = *(dynamic_cast<FSM<HungerLevel>*>(
		fsm_system_communicator->get_base_FSM(HUNGER_LEVEL_FSM_NAME)
	)->get_current_state());
	
	int next_state_int = static_cast<int>(
		((current_hunger_level == HungerLevel::FULL) || (current_hunger_level == HungerLevel::SOMEWHAT_HUNGRY))
		? OperationMode::EXPLORING : OperationMode::FINDING_FOOD
	);
	
	fsm->set_current_state(new OperationMode(OperationMode(next_state_int)));
	return current_state_int != next_state_int;
}

CurrentOperationModeController::CurrentOperationModeController(FSM<OperationMode>* f, FSMSystemCommunicator* fsc)
: FSMController(CURRENT_OPERATION_MODE_FSM_NAME, f, CURRENT_OPERATION_MODE_REEVALUATION_RATE_MILLISECONDS, fsc)
{
	// Do nothing
}

CurrentOperationModeController::~CurrentOperationModeController()
{
	// Do nothing
}