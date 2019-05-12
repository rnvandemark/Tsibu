#include "../include/I2CCommunicator.hpp"

I2CCommunicator::I2CCommunicator(std::string bus, int id)
{
	bus_URL = bus;
	device_id = id;

	fd_read = open(bus_URL.c_str(), O_RDONLY | O_NOCTTY | O_SYNC);
	if (fd_read < 0)
	{
		throw std::invalid_argument(std::string("Invalid filename provided [read]: ") + bus_URL);
	}
	if (ioctl(fd_read, I2C_SLAVE, device_id) < 0)
	{
		throw std::invalid_argument(std::string("Invalid device ID provided [read]: ") + std::to_string(device_id));
	}

	fd_write = open(bus_URL.c_str(), O_WRONLY | O_NOCTTY | O_SYNC);
	if (fd_write < 0)
	{
		throw std::invalid_argument(std::string("Invalid filename provided [write]: ") + bus_URL);
	}
	if (ioctl(fd_write, I2C_SLAVE, device_id) < 0)
	{
		throw std::invalid_argument(std::string("Invalid device ID provided [write]: ") + std::to_string(device_id));
	}
	
	std::cout << "[I2CCommunicator] I2C communication connected [" << fd_read << "][" << fd_write << "]" << std::endl;
	
	usleep(I2C_WAIT_AFTER_FILE_OPEN_MICROSECONDS);
}

I2CCommunicator::~I2CCommunicator()
{
	if (fd_read >= 0)
	{
		close(fd_read);
	}
	
	if (fd_write >= 0)
	{
		close(fd_write);
	}
}

int I2CCommunicator::get_device_id()
{
	return device_id;
}

std::string I2CCommunicator::get_bus_URL()
{
	return bus_URL;
}

std::string I2CCommunicator::read_message(unsigned int expected_size)
{
	std::string result = "";
	unsigned int total_bytes_read = 0;
	
	unsigned char read_buffer[I2C_READ_OPERATION_BUFFER_SIZE];
	
	while (total_bytes_read < expected_size)
	{
		int bytes_read = read(fd_read, read_buffer, I2C_READ_OPERATION_BUFFER_SIZE);
		if (bytes_read < 0)
		{
			std::cout << "[I2CCommunicator] Failure reading next read buffer." << std::endl;
			throw std::exception();
		}
		
		for (int i = 0; i < bytes_read; i++)
		{
			result += read_buffer[i];
		}
		
		total_bytes_read += bytes_read;
	}
	
	return result;
}

int I2CCommunicator::write_message(std::string message_contents)
{
	int written_bytes = write(fd_write, message_contents.c_str(), message_contents.size());
	if (written_bytes < 0)
	{
		std::cout << "[I2CCommunicator] Failure writing buffer." << std::endl;
		throw std::exception();
	}
	
	return written_bytes;
}