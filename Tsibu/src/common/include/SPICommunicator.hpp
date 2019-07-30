#ifndef TSIBU_SPI_COMMUNICATOR_HPP
#define TSIBU_SPI_COMMUNICATOR_HPP

#include <string>
#include <unistd.h>
#include <wiringPiSPI.h>

#include "SPIConstants.hpp"

/*
 *  This class is dedicated to creating a connection to and allowing for communication with a SPI device.
 *  This is accomplished through the SPI tools in the WiringPi library, and this is done to get more
 *  experience in using external libraries.
 */
class SPICommunicator
{
	private:

		/*
		 *  The filehandle for the device on the SPI bus.
		 */
		int fd;

		/*
		 *  The channel to use for SPI communcation.
		 */
		int channel_id;
		
		/*
		 *  A reusable buffer for message swapping.
		 */
		unsigned char message_buffer[SPI_MESSAGE_BUFFER_SIZE];

	public:

		/*
		 *  The constructor.
		 *  @param channel The channel to use for SPI communcation.
		 *  @param speed The SPI clock speed, in HZ. Must be between 500000 and 32000000, inclusive.
		 */
		explicit SPICommunicator(int channel, int speed);

		/*
		 *  The default constructor.
		 *  This uses the defualt values (in SPIConstants.hpp) for the channel and clock speed.
		 */
		explicit SPICommunicator();

		/*
		 *  The destructor.
		 *  Ensures the file IO descriptor is closed.
		 */
		~SPICommunicator();

		/*
		 *  Check for whether or not the filehandle describes an open file ready for IO.
		 *  @return Whether or not the filehandle was opened successfully.
		 */
		bool is_valid();

		/*
		 *  Write a message to the SPI device and grab its (automatic) response.
		 *  @param data The data to write to the SPI device's buffer.
		 *  @return The SPI device's response to the write operation.
		 */
		std::string swap_messages(std::string data);
};

#endif /* TSIBU_SPI_COMMUNICATOR_HPP */
