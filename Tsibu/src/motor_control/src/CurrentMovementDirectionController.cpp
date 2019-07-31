#include "../include/CurrentMovementDirectionController.hpp"

void CurrentMovementDirectionController::init()
{
	fsm->set_current_state(new MovementDirection(MovementDirection::NONE));
	
	fsm->set_variable<std::chrono::system_clock::time_point>(
		CURRENT_MOVEMENT_DIRECTION_VARIABLE_STATE_STATIC_UNTIL,
		new std::chrono::system_clock::time_point(std::chrono::system_clock::now())
	);
}

void CurrentMovementDirectionController::update_inputs()
{
	// Do nothing
}

bool CurrentMovementDirectionController::process()
{
	MovementDirection current_state = *(fsm->get_current_state());
	MovementDirection next_state;
	
	std::chrono::system_clock::time_point time_now = std::chrono::system_clock::now();
	if (time_now > dynamic_cast<FSMVariable<std::chrono::system_clock::time_point>*>(
		fsm->get_variable(CURRENT_MOVEMENT_DIRECTION_VARIABLE_STATE_STATIC_UNTIL)
	)->get())
	{
		next_state = MovementDirection::NONE;
		
		OperationMode current_operation_mode = *(dynamic_cast<FSM<OperationMode>*>(
			fsm_system_communicator->get_base_FSM(CURRENT_OPERATION_MODE_FSM_NAME)
		)->get_current_state());
		
		bool is_not_hungry = (current_operation_mode == OperationMode::IDLE) || (current_operation_mode == OperationMode::EXPLORING);
		
		switch(current_operation_mode)
		{
			case OperationMode::IDLE:
			case OperationMode::EXPLORING:
			case OperationMode::FINDING_FOOD:
			{
				SurroundingsAnalysis current_surroundings_analysis = *(dynamic_cast<FSM<SurroundingsAnalysis>*>(
					fsm_system_communicator->get_base_FSM(SURROUNDINGS_ANALYSIS_FSM_NAME)
				)->get_current_state());
				
				if (current_surroundings_analysis == SurroundingsAnalysis::KINETIC_OBJECT_IS_TOO_CLOSE)
				{
					if (current_state == MovementDirection::NONE)
					{
						next_state = MovementDirection::REVERSE;
					}
					else
					{
						next_state = MovementDirection::NONE;
					}
					
					fsm->set_variable<std::chrono::system_clock::time_point>(
						CURRENT_MOVEMENT_DIRECTION_VARIABLE_STATE_STATIC_UNTIL,
						new std::chrono::system_clock::time_point(
							time_now + std::chrono::milliseconds(CURRENT_MOVEMENT_DIRECTION_BACK_UP_TIME_MILLISECONDS)
						)
					);
					
					break;
				}
				
				if (is_not_hungry)
				{
					break;
				}
				
				HungerChangeMagnitude current_hunger_change_magnitude = *(dynamic_cast<FSM<HungerChangeMagnitude>*>(
					fsm_system_communicator->get_base_FSM(HUNGER_CHANGE_MAGNITUDE_FSM_NAME)
				)->get_current_state());
				
				if (current_hunger_change_magnitude == HungerChangeMagnitude::EXTREME_SATISFACTION)
				{
					next_state = MovementDirection::NONE;
				}
				else
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
					else if ((pr_center > (CURRENT_MOVEMENT_DIRECTION_FORWARD_FACTOR * pr_left))
						&& (pr_center > (CURRENT_MOVEMENT_DIRECTION_FORWARD_FACTOR * pr_right)))
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
						if ((current_state == MovementDirection::NONE) || (current_state == MovementDirection::RADIAL_CW))
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
						if ((current_state == MovementDirection::NONE) || (current_state == MovementDirection::RADIAL_CCW))
						{
							next_state = MovementDirection::RADIAL_CCW;
						}
						else
						{
							next_state = MovementDirection::NONE;
						}
					}
				}
				
				fsm->set_variable<std::chrono::system_clock::time_point>(
					CURRENT_MOVEMENT_DIRECTION_VARIABLE_STATE_STATIC_UNTIL,
					new std::chrono::system_clock::time_point(time_now)
				);
				
				break;
			}
		}
	}
	else
	{
		next_state = current_state;
	}
	
	fsm->set_current_state(new MovementDirection(next_state));
	if (static_cast<int>(current_state) != static_cast<int>(next_state))
	{
		if (!send_optimal_thunderborg_command(next_state))
		{
			std::cout << "[CurrentMovementDirectionController] Error sending ThunderBorg command." << std::endl;
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

bool CurrentMovementDirectionController::send_optimal_thunderborg_command(MovementDirection direction)
{
	switch(direction)
	{
		case MovementDirection::NONE:
			return tb_controller->stop_motors();
		
		case MovementDirection::FORWARD:
			return tb_controller->set_both_motor_speeds(THUNDERBORG_MOTOR_STRAIGHT_SPEED_MAGNITUDE_DEFAULT);
		
		case MovementDirection::REVERSE:
			return tb_controller->set_both_motor_speeds(-THUNDERBORG_MOTOR_STRAIGHT_SPEED_MAGNITUDE_DEFAULT);
		
		case MovementDirection::RADIAL_CW:
			return tb_controller->set_left_motor_speed(THUNDERBORG_MOTOR_TURN_SPEED_MAGNITUDE_DEFAULT)
				&& tb_controller->set_right_motor_speed(-THUNDERBORG_MOTOR_TURN_SPEED_MAGNITUDE_DEFAULT);
		
		case MovementDirection::RADIAL_CCW:
			return tb_controller->set_left_motor_speed(-THUNDERBORG_MOTOR_TURN_SPEED_MAGNITUDE_DEFAULT)
				&& tb_controller->set_right_motor_speed(THUNDERBORG_MOTOR_TURN_SPEED_MAGNITUDE_DEFAULT);
		
		default:
			throw std::invalid_argument(std::string("Invalid movement direction specified: ") + std::to_string(direction));
	}
}

CurrentMovementDirectionController::CurrentMovementDirectionController(FSM<MovementDirection>* f, FSMSystemCommunicator* fsc, SerialCommunicator* sc)
: FSMController(CURRENT_MOVEMENT_DIRECTION_FSM_NAME, f, CURRENT_MOVEMENT_DIRECTION_REEVALUATION_RATE_MILLISECONDS, fsc)
{
	serial_communicator = sc;
	tb_controller = new ThunderBorgController(false);
}

CurrentMovementDirectionController::~CurrentMovementDirectionController()
{
	delete tb_controller;
}
