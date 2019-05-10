#ifndef TSIBU_I2C_COMMUNICATOR_HPP
#define TSIBU_I2C_COMMUNICATOR_HPP

#include <iostream>
#include <string>

#include <wiringpi.h>
#include <wiringPiI2C.h>

/*
 *  This class is dedicated to creating a connection to and allowing for communication with an I2C device.
 *  It uses the WiringPi library, information can be found at http://wiringpi.com/reference/i2c-library/
 */
class I2CCommunicator
{
	private:

		/*
		 *  The ID for the I2C compatible device on the bus.
		 */
		int device_id;

		/*
		 *  The filehandle for the I2C bus.
		 */
		int fd;

	public:

		/*
		 *  The constructor.
		 *  @param id The ID for the I2C compatible device on the bus.
		 */
		explicit I2CCommunicator(int id);

		/*
		 *  Getter for the I2C device ID.
		 *  @return The I2C device ID.
		 */
		int get_device_id();
};

#endif /* TSIBU_I2C_COMMUNICATOR_HPP */