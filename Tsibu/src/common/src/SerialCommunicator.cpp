#include "../include/SerialCommunicator.hpp"

void SerialCommunicator::purge_buffer()
{
	unsigned char purge_buffer[SERIAL_PURGE_BUFFER_SIZE];

	while (true)
	{
		int bytes_read = read(fd, purge_buffer, SERIAL_PURGE_BUFFER_SIZE);
		if (bytes_read < 0)
		{
			std::cout << "[SerialCommunicator] Error purging buffers." << std::endl;
			throw std::exception();
		}
		else if (bytes_read == 0)
		{
			return;
		}
	}
}

std::string SerialCommunicator::read_message()
{
	unsigned char initial_read_buffer[1];
	int total_bytes_read = 0;
	
	int expected_bytes_remaining = -1;
	
	while (total_bytes_read < 2)
	{
		int bytes_read = read(fd, initial_read_buffer, 1);
		if (bytes_read < 0)
		{
			std::cout << "[SerialCommunicator] Failure reading initial read buffer #0." << std::endl;
			throw std::exception();
		}
		else if (bytes_read == 0)
		{
			continue;
		}
		
		if (total_bytes_read == 0)
		{
			if (initial_read_buffer[0] != SERIAL_START_RXTX_RESPONSE)
			{
				std::cout << "[SerialCommunicator] Failure reading initial read buffer #1, got "
					<< initial_read_buffer[0] << " (expecting " << SERIAL_START_RXTX_RESPONSE << ")." << std::endl;
				throw std::exception();
			}
		}
		else if (total_bytes_read == 1)
		{
			expected_bytes_remaining = static_cast<int>(initial_read_buffer[0]);
		}
		
		total_bytes_read += bytes_read;
	}

	std::string result = "";
	total_bytes_read = 0;
	
	unsigned char read_buffer[SERIAL_READ_OPERATION_BUFFER_SIZE];
	
	while (total_bytes_read < expected_bytes_remaining)
	{
		int bytes_read = read(fd, read_buffer, SERIAL_READ_OPERATION_BUFFER_SIZE);
		if (bytes_read < 0)
		{
			std::cout << "[SerialCommunicator] Failure reading next read buffer." << std::endl;
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

int SerialCommunicator::write_message(std::string message_contents)
{
	int written_bytes = write(fd, message_contents.c_str(), message_contents.size());
	if (written_bytes < 0)
	{
		std::cout << "[SerialCommunicator] Failure writing buffer." << std::endl;
		throw std::exception();
	}
	
	return written_bytes;
}

uint16_t SerialCommunicator::extract_value_from_response_given_descriptor(std::string message, std::string expected_descriptor)
{
	std::size_t pos_descriptor_start = message.find(SERIAL_START_RXTX_DESCRIPTOR);
	std::size_t pos_value_start = message.find(SERIAL_END_RXTX_DESCRIPTOR);
	std::size_t pos_message_scope_end = message.find(SERIAL_END_RXTX);

	if ((pos_descriptor_start == std::string::npos) || (pos_value_start == std::string::npos) || (pos_message_scope_end == std::string::npos))
	{
		std::cout << "[SerialCommunicator] Failure extracting value from descriptor #0." << std::endl;
		throw std::exception();
	}

	int descriptor_start_index = static_cast<int>(pos_descriptor_start) + 1;
	int descriptor_end_index   = static_cast<int>(pos_value_start);

	if (message.substr(descriptor_start_index, descriptor_end_index - descriptor_start_index).compare(expected_descriptor) != 0)
	{
		std::cout << "[SerialCommunicator] Failure extracting value from descriptor #1." << std::endl;
		throw std::exception();
	}

	int value_start_index = static_cast<int>(pos_value_start) + 1;
	int value_end_index   = static_cast<int>(pos_message_scope_end);
	
	int value = static_cast<uint16_t>(std::stoi(message.substr(value_start_index, value_end_index - value_start_index)));
	if (value > SERIAL_MAX_ANALOG_VALUE)
	{
		std::cout << "[SerialCommunicator] Failure extracting value from descriptor #2." << std::endl;
		throw std::exception();
	}
	
	return value;
}

uint16_t SerialCommunicator::get_value_for_descriptor(std::string descriptor)
{
	std::string formatted_string(1, SERIAL_START_RXTX_READ_REQUEST);
	formatted_string += std::string(1, static_cast<unsigned char>(0));
	formatted_string += SERIAL_START_RXTX_DESCRIPTOR;
	formatted_string += descriptor;
	formatted_string += SERIAL_END_RXTX_DESCRIPTOR;
	formatted_string += SERIAL_END_RXTX;
	
	formatted_string[1] = static_cast<unsigned char>(formatted_string.size() - 2);
	
	io_mutex.lock();
	int written_bytes = write_message(formatted_string);
	if (written_bytes != formatted_string.size())
	{
		std::cout << "[SerialCommunicator] Failure getting value for descriptor." << std::endl;
		throw std::exception();
	}
	
	std::string full_response = read_message();
	io_mutex.unlock();
	
	return extract_value_from_response_given_descriptor(full_response, descriptor);
}

bool SerialCommunicator::set_value_for_descriptor(std::string descriptor, std::string byte_values)
{
	std::string formatted_string(1, SERIAL_START_RXTX_WRITE_REQUEST);
	formatted_string += std::string(1, static_cast<unsigned char>(0));
	formatted_string += SERIAL_START_RXTX_DESCRIPTOR;
	formatted_string += descriptor;
	formatted_string += SERIAL_END_RXTX_DESCRIPTOR;
	formatted_string += byte_values;
	formatted_string += SERIAL_END_RXTX;
		
	formatted_string[1] = static_cast<unsigned char>(formatted_string.size() - 2);
	
	io_mutex.lock();
	int written_bytes = write_message(formatted_string);
	if (written_bytes != formatted_string.size())
	{
		std::cout << "[SerialCommunicator] Failure setting value for descriptor." << std::endl;
		throw std::exception();
	}

	std::string full_response = read_message();
	io_mutex.unlock();
	
	return extract_value_from_response_given_descriptor(full_response, descriptor) == 1;
}

SerialCommunicator::SerialCommunicator(std::string name, BaudRate baud, int parity)
{
	fd = open(name.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
		throw std::invalid_argument(std::string("Invalid filename provided: ") + name);
	}
	
	std::cout << "[SerialCommunicator] Serial communication connected [" << fd << "]" << std::endl;
	
	usleep(SERIAL_WAIT_AFTER_FILE_OPEN_MICROSECONDS);
	
	set_interface_attributes(baud, parity);
	set_blocking(false);
	
	purge_buffer();
}

SerialCommunicator::~SerialCommunicator()
{
	if (fd >= 0)
	{
		close(fd);
	}
}

void SerialCommunicator::set_blocking(bool should_block)
{
	struct termios tty;
	memset(&tty, 0, sizeof tty);
	if (tcgetattr(fd, &tty) != 0)
	{
		std::cout << "[SerialCommunicator] Failure extracting value from descriptor #2." << std::endl;
		throw std::exception();
	}
	
	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = SERIAL_READ_TIMEOUT_HUNDREDS_OF_MILLISECONDS;
	
	if (tcsetattr(fd, TCSANOW, &tty) != 0)
	{
		throw std::exception();
	}
}

void SerialCommunicator::set_interface_attributes(BaudRate baud, int parity)
{
	struct termios tty;
	memset(&tty, 0, sizeof tty);
	if (tcgetattr(fd, &tty) != 0)
	{
		throw std::exception();
	}
	
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~CRTSCTS;
	tty.c_cflag |= (CLOCAL | CREAD);

	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO;
	tty.c_lflag &= ~ECHOE;
	tty.c_lflag &= ~ECHONL;
	tty.c_lflag &= ~ISIG;

	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

	tty.c_oflag &= ~OPOST;
	tty.c_oflag &= ~ONLCR;
	
	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = SERIAL_READ_TIMEOUT_HUNDREDS_OF_MILLISECONDS;
	
	cfsetispeed(&tty, static_cast<int>(baud));
	cfsetospeed(&tty, static_cast<int>(baud));
	
	//tty.c_cflag |= parity;
	
	if (tcsetattr(fd, TCSANOW, &tty) != 0)
	{
		throw std::exception();
	}
}

bool SerialCommunicator::get_digital_value_for_descriptor(std::string descriptor)
{
	return get_value_for_descriptor(descriptor) == 1;
}

uint16_t SerialCommunicator::get_analog_value_for_descriptor(std::string descriptor)
{
	return get_value_for_descriptor(descriptor);
}

bool SerialCommunicator::set_digital_value_for_descriptor(std::string descriptor, bool new_value_is_high)
{
	return set_value_for_descriptor(descriptor, std::string(1, new_value_is_high ? SERIAL_DIGITAL_VALUE_HIGH : SERIAL_DIGITAL_VALUE_LOW));
}
		
bool SerialCommunicator::set_analog_value_for_descriptor(std::string descriptor, uint16_t new_analog_value)
{
	return set_value_for_descriptor(descriptor, std::to_string(new_analog_value));
}