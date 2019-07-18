#include "../include/SPICommunicator.hpp"

SPICommunicator::SPICommunicator(int channel, int speed)
{
	channel_id = channel;
	fd = wiringPiSPISetup(channel_id, speed);
}

SPICommunicator::SPICommunicator()
: SPICommunicator(SPI_DEFAULT_CHANNEL, SPI_DEFAULT_BUS_SPEED)
{
	// Does nothing extra
}

SPICommunicator::~SPICommunicator()
{
	if (is_valid())
	{
		close(fd);
	}
}

bool SPICommunicator::is_valid()
{
	return fd >= 0;
}

std::string SPICommunicator::swap_messages(std::string data)
{
	data.copy(reinterpret_cast<char*>(message_buffer), data.length());
	int response_size = wiringPiSPIDataRW(channel_id, message_buffer, data.length());
	return std::string(message_buffer, message_buffer + response_size);
}
