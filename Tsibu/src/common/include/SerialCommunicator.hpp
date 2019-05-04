#ifndef TSIBU_SERIAL_COMMUNICATOR_HPP
#define TSIBU_SERIAL_COMMUNICATOR_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

#include "BaudRate.hpp"

#define START_RX_TX static_cast<unsigned char>('~')
#define START_RX_TX_DESCRIPTOR static_cast<unsigned char>('!')
#define END_RX_TX_DESCRIPTOR static_cast<unsigned char>('@')
#define END_RX_TX static_cast<unsigned char>('#')

#define WAIT_AFTER_FILE_OPEN_MICROSECONDS 5000000

#define READ_TIMEOUT_HUNDREDS_OF_MILLISECONDS 3

#define READ_OPERATION_BUFFER_SIZE 16

class SerialCommunicator
{
	private:
		
		/*
		 *  The descriptor of the file utilized for the serial communication stream.
		 */
		int fd;
		
		/*
		 *  Read a message from the file stream, meaning capture the bytes until the transmission end character is seen.
		 *  @return The string captured from the file stream.
		 */
		std::string read_message();
		
		/*
		 *  Send a message through the file stream.
		 *  @param message_contents A string describing the contents of the message.
		 *  @return The number of bytes written, -1 on failure.
		 */
		int write_message(std::string message_contents);
	
	public:
		
		/*
		 *  The main constructor.
		 *  Establishes connection with the stream and reserves the file descriptor.
		 *  @param name The url of the file name to establish a connection to.
		 *  @param baud An enum specifying the baud rate to expect for communication.
		 *  @param parity The parity bits to expect for communication.
		 */
		explicit SerialCommunicator(std::string name, BaudRate baud, int parity);
		
		/*
		 *  The destructor.
		 *  Closes any existing connection.
		 */
		~SerialCommunicator();
		
		/*
		 *  Set the IO operations on the file stream to be blocking or non-blocking.
		 *  @param should_block Whether or not communications should be blocking operations.
		 */
		void set_blocking(bool should_block);
		
		/*
		 *  A utility function to set the interface attributes for the file stream.
		 *  @param baud An enum specifying the baud rate to expect for communication.
		 *  @param parity The parity bits to expect for communication.
		 */
		void set_interface_attributes(BaudRate baud, int parity);
		
		/*
		 *  Query the serial device with a descriptor and expect a quantifiable value as a response, given some data type to cast to.
		 *  @param descriptor The descriptor of the data requested.
		 *  @return The contents of the message, casted to the desired data type.
		 */
		template <typename DataType>
		DataType get_value_for_descriptor(std::string descriptor)
		{
			std::string formatted_string = "";
			formatted_string += START_RX_TX;
			formatted_string += std::string(1, static_cast<unsigned char>(0));
			formatted_string += START_RX_TX_DESCRIPTOR;
			formatted_string += descriptor;
			formatted_string += END_RX_TX_DESCRIPTOR;
			formatted_string += END_RX_TX;
			
			formatted_string[1] = static_cast<unsigned char>(formatted_string.size() - 2);
			
			int written_bytes = write_message(formatted_string);
			if (written_bytes != formatted_string.size())
			{
				throw std::exception();
			}
			
			std::string full_message = read_message();
			std::size_t pos_value_start = full_message.find(END_RX_TX_DESCRIPTOR);
			std::size_t pos_message_scope_end = full_message.find(END_RX_TX);
			
			if ((pos_value_start == std::string::npos) || (pos_message_scope_end == std::string::npos))
			{
				throw std::exception();
			}
			
			int start_index = static_cast<int>(pos_value_start) + 1;
			int end_index   = static_cast<int>(pos_message_scope_end);
			
			return static_cast<DataType>(std::stol(full_message.substr(start_index, end_index - start_index)));
		}
};

#endif /* TSIBU_SERIAL_COMMUNICATOR_HPP */