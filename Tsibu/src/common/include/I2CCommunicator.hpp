#ifndef TSIBU_I2C_COMMUNICATOR_HPP
#define TSIBU_I2C_COMMUNICATOR_HPP

#include <string>
#include <iostream>
#include <type_traits>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include "I2CConstants.hpp"

/*
 *  This class is dedicated to creating a connection to and allowing for communication with an I2C device.
 */
class I2CCommunicator
{
	private:

		/*
		 *  The file URL for the I2C bus that the device resides on.
		 */
		std::string bus_URL;

		/*
		 *  The ID for the I2C compatible device on the bus.
		 */
		int device_id;

		/*
		 *  The read filehandle for the device on the I2C bus.
		 */
		int fd_read;

		/*
		 *  The write filehandle for the device on the I2C bus.
		 */
		int fd_write;

	public:

		/*
		 *  The constructor.
		 *  @param bus The file URL for the I2C bus that the device resides on.
		 *  @param id The ID for the I2C compatible device on the bus.
		 */
		explicit I2CCommunicator(std::string bus, int id);

		/*
		 *  The destructor.
		 *  Ensures that both file IO descriptors are closed.
		 */
		~I2CCommunicator();

		/*
		 *  Getter for the I2C bus URL.
		 *  @return The I2C bus URL.
		 */
		std::string get_bus_URL();

		/*
		 *  Getter for the I2C device ID.
		 *  @return The I2C device ID.
		 */
		int get_device_id();

		/*
		 *  Utility function to read a specified number of bytes off of the I2C bus.
		 *  @return The value heard off of the bus.
		 */
		std::string read_message(unsigned int expected_size);

		/*
		 *  Utility function to write a variable number of bytes to the I2C bus.
		 *  @param new_data The data to write.
		 *  @return The number of bytes successfully written.
		 */
		int write_message(std::string new_data);
};

#endif /* TSIBU_I2C_COMMUNICATOR_HPP */