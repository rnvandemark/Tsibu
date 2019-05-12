#include "../include/ThunderBorgController.hpp"

std::string ThunderBorgController::read_I2C_bus_raw(unsigned int expected_length)
{
	return i2c_communicator->read_message(expected_length);
}

bool ThunderBorgController::write_I2C_bus_raw(std::string new_data)
{
	return i2c_communicator->write_message(new_data) == new_data.length();
}

bool ThunderBorgController::set_motor_speeds(MotorType motor_type, double s)
{
	unsigned char cmd_direction;
	
	if (s < 0)
	{
		s = -s;
		
		switch(motor_type)
		{
			case MotorType::LEFT:
				cmd_direction = THUNDERBORG_CMD_SET_LEFT_MOTOR_REVERSE;
				break;
			
			case MotorType::RIGHT:
				cmd_direction = THUNDERBORG_CMD_SET_RIGHT_MOTOR_REVERSE;
				break;
			
			case MotorType::BOTH:
				cmd_direction = THUNDERBORG_CMD_SET_BOTH_MOTORS_REVERSE;
				break;
			
			default:
				throw std::invalid_argument(std::string("Invalid motor type provided: ") + std::to_string(motor_type));
		}
	}
	else
	{
		switch(motor_type)
		{
			case MotorType::LEFT:
				cmd_direction = THUNDERBORG_CMD_SET_LEFT_MOTOR_FORWARD;
				break;
			
			case MotorType::RIGHT:
				cmd_direction = THUNDERBORG_CMD_SET_RIGHT_MOTOR_FORWARD;
				break;
			
			case MotorType::BOTH:
				cmd_direction = THUNDERBORG_CMD_SET_BOTH_MOTORS_FORWARD;
				break;
			
			default:
				throw std::invalid_argument(std::string("Invalid motor type provided: ") + std::to_string(motor_type));
		}
	}
	
	unsigned char int_s = static_cast<int>(s * THUNDERBORG_PWM_MAX);
	if (int_s < 0)
	{
		int_s = 0;
	}
	else if (int_s > THUNDERBORG_PWM_MAX)
	{
		int_s = THUNDERBORG_PWM_MAX;
	}
	unsigned char fixed_s = static_cast<unsigned char>(int_s);
	
	return write_I2C_bus_raw(std::string({cmd_direction, fixed_s}));
}

ThunderBorgController::ThunderBorgController(bool set_motor_failsafe)
{
	i2c_communicator = new I2CCommunicator(THUNDERBORG_I2C_BUS_URL_DEFAULT, THUNDERBORG_I2C_DEVICE_ID_DEFAULT);
	
	if (!write_I2C_bus_raw(std::string({THUNDERBORG_CMD_SET_MOTOR_FAILSAFE, set_motor_failsafe ? THUNDERBORG_CMD_VALUE_ON : THUNDERBORG_CMD_VALUE_OFF})))
	{
		std::cout << "[ThunderBorgController] Error setting motor failsafe!" << std::endl;
	}
	
	if (!set_LED(1.0, 0.2, 0))
	{
		std::cout << "[ThunderBorgController] Error setting pretty orange light on startup." << std::endl;
	}
}

ThunderBorgController::~ThunderBorgController()
{
	if (!set_both_motor_speeds(0))
	{
		std::cout << "[ThunderBorgController] Error setting motor speeds to 0 on shutdown!" << std::endl;
	}
	
	if (!set_LED(0, 0, 0))
	{
		std::cout << "[ThunderBorgController] Error turning off LED on shutdown." << std::endl;
	}
	
	delete i2c_communicator;
}

std::vector<double> ThunderBorgController::get_LED()
{
	if (!write_I2C_bus_raw(std::string({THUNDERBORG_CMD_GET_LED})))
	{
		std::cout << "[ThunderBorgController] Error writing get LED value request." << std::endl;
		throw std::exception();
	}
	
	std::string response = read_I2C_bus_raw(4);
	if (response[0] != THUNDERBORG_CMD_GET_LED)
	{
		std::cout << "[ThunderBorgController] Error reading get LED value response." << std::endl;
		throw std::exception();	
	}
	
	std::vector<double> LED_values(3);
	LED_values[0] = static_cast<double>(response[1]) / THUNDERBORG_PWM_MAX;
	LED_values[1] = static_cast<double>(response[2]) / THUNDERBORG_PWM_MAX;
	LED_values[2] = static_cast<double>(response[3]) / THUNDERBORG_PWM_MAX;
	
	return LED_values;
}

bool ThunderBorgController::set_LED(double r, double g, double b)
{
	unsigned char int_r = static_cast<int>(r * THUNDERBORG_PWM_MAX);
	if (int_r < 0)
	{
		int_r = 0;
	}
	else if (int_r > THUNDERBORG_PWM_MAX)
	{
		int_r = THUNDERBORG_PWM_MAX;
	}
	unsigned char fixed_r = static_cast<unsigned char>(int_r);
	
	unsigned char int_g = static_cast<int>(g * THUNDERBORG_PWM_MAX);
	if (int_g < 0)
	{
		int_g = 0;
	}
	else if (int_g > THUNDERBORG_PWM_MAX)
	{
		int_g = THUNDERBORG_PWM_MAX;
	}
	unsigned char fixed_g = static_cast<unsigned char>(int_g);
	
	unsigned char int_b = static_cast<int>(b * THUNDERBORG_PWM_MAX);
	if (int_b < 0)
	{
		int_b = 0;
	}
	else if (int_b > THUNDERBORG_PWM_MAX)
	{
		int_b = THUNDERBORG_PWM_MAX;
	}
	unsigned char fixed_b = static_cast<unsigned char>(int_b);
	
	return write_I2C_bus_raw(std::string({THUNDERBORG_CMD_SET_LED, fixed_r, fixed_g, fixed_b}));
}

bool ThunderBorgController::set_left_motor_speed(double s)
{
	return set_motor_speeds(MotorType::LEFT, s);
}

bool ThunderBorgController::set_right_motor_speed(double s)
{
	return set_motor_speeds(MotorType::RIGHT, s);
}

bool ThunderBorgController::set_both_motor_speeds(double s)
{
	return set_motor_speeds(MotorType::BOTH, s);
}