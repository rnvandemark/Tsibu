#include "../include/SerialCommunicator.hpp"

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
			throw std::exception();
		}
		
		if (total_bytes_read == 0)
		{
			if (initial_read_buffer[0] != START_RX_TX)
			{
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
	
	unsigned char read_buffer[READ_OPERATION_BUFFER_SIZE];
	while (total_bytes_read < expected_bytes_remaining)
	{
		int bytes_read = read(fd, read_buffer, READ_OPERATION_BUFFER_SIZE);
		if (bytes_read < 0)
		{
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
		throw std::exception();
	}
	
	return written_bytes;
}

SerialCommunicator::SerialCommunicator(std::string name, BaudRate baud, int parity)
{
	fd = open(name.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
		throw std::invalid_argument(std::string("Invalid filename provided: ") + name);
	}
	
	std::cout << "Serial communication connected [" << fd << "]" << std::endl;
	
	usleep(WAIT_AFTER_FILE_OPEN_MICROSECONDS);
	
	set_interface_attributes(baud, parity);
	set_blocking(false);
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
		throw std::exception();
	}
	
	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = READ_TIMEOUT_HUNDREDS_OF_MILLISECONDS;
	
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
	tty.c_cc[VTIME] = READ_TIMEOUT_HUNDREDS_OF_MILLISECONDS;
	
	cfsetispeed(&tty, static_cast<int>(baud));
	cfsetospeed(&tty, static_cast<int>(baud));
	
	//tty.c_cflag |= parity;
	
	if (tcsetattr(fd, TCSANOW, &tty) != 0)
	{
		throw std::exception();
	}
}