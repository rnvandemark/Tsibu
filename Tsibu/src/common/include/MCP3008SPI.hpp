#ifndef TSIBU_MCP3008_SPI_HPP
#define TSIBU_MCP3008_SPI_HPP

#include <string>

#include "SPICommunicator.hpp"
#include "SPIConstants.hpp"

/*
 *  This class is for creating a means of SPI communication for an MCP3008 ADC.
 */
class MCP3008SPI
{
	private:

		/*
		 *  The SPI filehandle.
		 */
		SPICommunicator* SPI_communicator;

	public:

		/*
		 *  The constructor.
		 *  @param channel The channel to use for SPI communcation.
		 *  @param speed The SPI clock speed, in HZ. Must be between 500000 and 32000000, inclusive.
		 */
		explicit MCP3008SPI(int channel, int speed);

		/*
		 *  The default constructor.
		 *  This uses the defualt values (in SPIConstants.hpp) for the channel and clock speed.
		 */
		explicit MCP3008SPI();

		/*
		 *  The destructor.
		 *  Ensures the filehandle is closed.
		 */
		~MCP3008SPI();

		/*
		 *  Check for whether or not the filehandle describes valid SPI IO.
		 *  @return Whether or not the filehandle is ready for SPI.
		 */
		bool is_valid();

		/*
		 *  Read the current digitally converted value.
		 *  @param channel_number The channel number to read.
		 *  @return The value on the channel.
		 */
		uint16_t read_channel(uint8_t channel_number);
};

#endif /* TSIBU_MCP3008_SPI_HPP */
