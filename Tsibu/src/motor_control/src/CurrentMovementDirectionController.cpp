#include "../include/CurrentMovementDirectionController.hpp"

void CurrentMovementDirectionController::init()
{
	fsm->set_current_state(new MovementDirection(MovementDirection::NONE));
	
	update_inputs();
}

void CurrentMovementDirectionController::update_inputs()
{
	fsm->set_variable<std::chrono::system_clock::time_point>(
		CURRENT_MOVEMENT_DIRECTION_VARIABLE_STATE_STATIC_UNTIL,
		new std::chrono::system_clock::time_point(std::chrono::system_clock::now())
	);
}

bool CurrentMovementDirectionController::process()
{
	MovementDirection current_state = *(fsm->get_current_state());
	MovementDirection next_state = MovementDirection::NONE;
	
	std::chrono::system_clock::time_point time_now = std::chrono::system_clock::now();
	if (time_now > dynamic_cast<FSMVariable<std::chrono::system_clock::time_point>*>(fsm->get_variable(CURRENT_MOVEMENT_DIRECTION_VARIABLE_STATE_STATIC_UNTIL))->get())
	{
		OperationMode current_operation_mode = *(dynamic_cast<FSM<OperationMode>*>(
			fsm_system_communicator->get_base_FSM(CURRENT_OPERATION_MODE_FSM_NAME)
		)->get_current_state());
		
		switch(current_operation_mode)
		{
			case OperationMode::FINDING_FOOD:
			{
				uint16_t pr_left   = serial_communicator->get_analog_value_for_descriptor(HUNGER_CHANGE_MAGNITUDE_LEFT_PR_DESCRIPTOR);
				uint16_t pr_center = serial_communicator->get_analog_value_for_descriptor(HUNGER_CHANGE_MAGNITUDE_CENTER_PR_DESCRIPTOR);
				uint16_t pr_right  = serial_communicator->get_analog_value_for_descriptor(HUNGER_CHANGE_MAGNITUDE_RIGHT_PR_DESCRIPTOR);
				uint16_t pr_rear   = serial_communicator->get_analog_value_for_descriptor(HUNGER_CHANGE_MAGNITUDE_REAR_PR_DESCRIPTOR);
				
				if ((pr_rear > pr_left) && (pr_rear > pr_center) && (pr_rear > pr_right))
				{
					if ((current_state == MovementDirection::FORWARD) || (current_state == MovementDirection::REVERSE))
					{
						next_state = MovementDirection::NONE;
					}
					else if (pr_left > pr_right)
					{
						if (current_state == MovementDirection::RADIAL_CW)
						{
							next_state = MovementDirection::NONE;
						}
						else
						{
							next_state = MovementDirection::RADIAL_CCW;
						}
					}
					else
					{
						if (current_state == MovementDirection::RADIAL_CCW)
						{
							next_state = MovementDirection::NONE;
						}
						else
						{
							next_state = MovementDirection::RADIAL_CW;
						}
					}
				}
				else if ((pr_center > pr_left) && (pr_center > pr_right))
				{
					if ((current_state == MovementDirection::FORWARD) || (current_state == MovementDirection::NONE))
					{
						next_state = MovementDirection::FORWARD;
					}
					else
					{
						next_state = MovementDirection::NONE;
					}
				}
				else if (pr_right > pr_left)
				{
					if (current_state == MovementDirection::RADIAL_CW)
					{
						next_state = MovementDirection::RADIAL_CW;
					}
					else
					{
						next_state = MovementDirection::NONE;
					}
				}
				else
				{
					if (current_state == MovementDirection::RADIAL_CCW)
					{
						next_state = MovementDirection::RADIAL_CCW;
					}
					else
					{
						next_state = MovementDirection::NONE;
					}
				}
				
				break;
			}
		}
		
		fsm->set_variable<std::chrono::system_clock::time_point>(
			CURRENT_MOVEMENT_DIRECTION_VARIABLE_STATE_STATIC_UNTIL,
			new std::chrono::system_clock::time_point(time_now)
		);
	}
	
	fsm->set_current_state(new MovementDirection(next_state));
	return static_cast<int>(current_state) != static_cast<int>(next_state);
}

CurrentMovementDirectionController::CurrentMovementDirectionController(FSM<MovementDirection>* f, FSMSystemCommunicator* fsc, SerialCommunicator* sc)
: FSMController(CURRENT_MOVEMENT_DIRECTION_FSM_NAME, f, CURRENT_MOVEMENT_DIRECTION_REEVALUATION_RATE_MILLISECONDS, fsc)
{
	serial_communicator = sc;
}

CurrentMovementDirectionController::~CurrentMovementDirectionController()
{
	// Do nothing
}