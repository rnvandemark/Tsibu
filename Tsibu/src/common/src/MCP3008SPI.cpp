#include "../include/MCP3008SPI.hpp"

MCP3008SPI::MCP3008SPI(int channel, int speed)
{
	SPI_communicator = new SPICommunicator(channel, speed);
}

MCP3008SPI::MCP3008SPI()
: MCP3008SPI(SPI_DEFAULT_CHANNEL, SPI_DEFAULT_BUS_SPEED)
{
	// Do nothing else
}

MCP3008SPI::~MCP3008SPI()
{
	if (SPI_communicator != nullptr)
	{
		delete SPI_communicator;
	}
}
	
bool MCP3008SPI::is_valid()
{
	return (SPI_communicator != nullptr)
		&& (SPI_communicator->is_valid());
}

uint16_t MCP3008SPI::read_channel(uint8_t channel_number)
{
	std::string command = std::string(1, 192 | ((channel_number & 0x7) << 3)) + std::string(2, 0);
	std::string response = SPI_communicator->swap_messages(command);
	
	uint16_t result = (response[0] & 0x01) << 9;
	result |= (response[1] & 0xFF) << 1;
	result |= (response[2] & 0x80) >> 7;
	
	return result;
}
