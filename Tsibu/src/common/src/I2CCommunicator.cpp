#include "../include/I2CCommunicator.hpp"

I2CCommunicator::I2CCommunicator(int id)
{
	device_id = id;

	wiringPiSetup();

	fd = wiringPiI2CSetup(device_id);
	if (fd < 0)
	{
		throw std::invalid_argument(std::string("Invalid I2C device ID provided: ") + std::to_string(device_id));
	}

	std::cout << "fd: " << fd << std::endl;
}

int I2CCommunicator::get_device_id()
{
	return device_id;
}