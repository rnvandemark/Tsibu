#ifndef TSIBU_SERIAL_COMMUNICATOR_HPP
#define TSIBU_SERIAL_COMMUNICATOR_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <mutex>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

#include "SerialConstants.hpp"
#include "BaudRate.hpp"

class SerialCommunicator
{
	private:
		
		/*
		 *  The descriptor of the file utilized for the serial communication stream.
		 */
		int fd;
		
		/*
		 *  A mutex to prevent reading and writing too close to one another.
		 */
		std::mutex io_mutex;
		
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
		
		/*
		 *  Given a message, get the value corresponsing to the descriptor expected.
		 *  @param message The packet to parse for the value.
		 *  @param expected_descriptor The descriptor to check for, to confirm proper functionality.
		 *  @return The value associated with the provided descriptor.
		 */
		uint16_t extract_value_from_response_given_descriptor(std::string message, std::string expected_descriptor);
		
		/*
		 *  Query the serial device with a descriptor and expect a value as a response.
		 *  @param descriptor The descriptor of the data requested.
		 *  @return The contents of the message, casted to the required short data type.
		 */
		uint16_t get_value_for_descriptor(std::string descriptor);
		
		/*
		 *  Create and send the request for and then return the result of writing to the serial stream a message to set the value of a descriptor.
		 *  @param descriptor The descriptor for the value to set.
		 *  @param byte_values The byte values for the other end of the serial stream to handle.
		 *  @return Wether or not the write operation was successful.
		 */
		bool set_value_for_descriptor(std::string descriptor, std::string byte_values);
		
		/*
		 *  A helper function to purge all the bytes waiting in the serial buffer.
		 */
		void purge_buffer();
	
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
		 *  Query the serial device with a descriptor and expect a digital value as a response.
		 *  @param descriptor The descriptor of the data requested.
		 *  @return The digital value associated with the descriptor, true and false being HIGH and LOW, respectively.
		 */
		bool get_digital_value_for_descriptor(std::string descriptor);
		
		/*
		 *  Query the serial device with a descriptor and expect an analog value as a response.
		 *  @param descriptor The descriptor of the data requested.
		 *  @return The contents of the message, casted to the required short data type.
		 */
		uint16_t get_analog_value_for_descriptor(std::string descriptor);
		
		/*
		 *  Create and send the request for and then return the result of writing to the serial stream a message to set the digital value of a descriptor.
		 *  @param descriptor The descriptor for the value to set.
		 *  @param new_value_is_high Whether or not to set the digital input as HIGH or LOW.
		 *  @return Wether or not the write operation was successful.
		 */
		bool set_digital_value_for_descriptor(std::string descriptor, bool new_value_is_high);
		
		/*
		 *  Create and send the request for and then return the result of writing to the serial stream a message to set the analog value of a descriptor.
		 *  @param descriptor The descriptor for the value to set.
		 *  @param new_analog_value The analog value to set the analog input as.
		 *  @return Wether or not the write operation was successful.
		 */
		bool set_analog_value_for_descriptor(std::string descriptor, uint16_t new_analog_value);
};

#endif /* TSIBU_SERIAL_COMMUNICATOR_HPP */