#ifndef TSIBU_THUNDERBORG_CONTROLLER_HPP
#define TSIBU_THUNDERBORG_CONTROLLER_HPP

#include <string>
#include <vector>
#include <stdexcept>

#include "../../common/include/I2CCommunicator.hpp"

#include "ThunderBorgConstants.hpp"
#include "MotorType.hpp"

/*
 *  This class is dedicated to creating a communicator for a ThunderBorg motor controller unit.
 */
class ThunderBorgController
{
	private:

		/*
		 *  The I2C communication handler.
		 */
		I2CCommunicator* i2c_communicator;

		/*
		 *  Utility function to read a certain number of bytes from the ThunderBorg on the I2C bus.
		 *  @param expected_length The number of bytes that are expected to return from the bus.
		 *  @param timeout_ms The number of milliseconds to time out on for this read operation.
		 *  @return The string of bytes that were heard.
		 */
		std::string read_I2C_bus_raw(unsigned int expected_length);

		/*
		 *  Utility function to write a variable number of bytes onto the I2C bus for the ThunderBorg.
		 *  @param new_data The bytes to write.
		 *  @return Whether or not the write operation was successful.
		 */
		bool write_I2C_bus_raw(std::string new_data);
		
		/*
		 *  Utility function to calculate the set of motors to set motor speed for, either forward or backwards.
		 *  @param s The proportion of the max speed to set the motors to, in the range [-1.0, 1.0].
		 *  @return Whether or not the motor speeds were correctly set.
		 */
		bool set_motor_speeds(MotorType motor_type, double s);

	public:

		/*
		 *  The constructor.
		 *  Instantiates the I2C communicator.
		 *  @param set_motor_failsafe Whether or not the motor failsafe should be activated on startup.
		 */
		explicit ThunderBorgController(bool set_motor_failsafe);

		/*
		 *  The destructor.
		 *  Frees the memory from the I2C communicator.
		 */
		~ThunderBorgController();

		/*
		 *  Get the LED on the thunderborg controller as a combination of magnitudes for the R, G, and B components.
		 *  @return A vector of decimal color intensities.
		 */
		std::vector<double> get_LED();

		/*
		 *  Set the LED on the thunderborg controller to a combination of magnitudes for the R, G, and B components.
		 *  @param r The red intensity of the composite color, in the range [0.0, 1.0].
		 *  @param g The green intensity of the composite color, in the range [0.0, 1.0].
		 *  @param b The blue intensity of the composite color, in the range [0.0, 1.0].
		 *  @return Whether or not the LED's color was set successfully.
		 */
		bool set_LED(double r, double g, double b);
		
		/*
		 *  Set the left motors to a specified speed.
		 *  @param s The proportion of the max speed to set the motors to, in the range [-1.0, 1.0].
		 *  @return Whether or not the motor speed was successfully set.
		 */
		bool set_left_motor_speed(double s);
		
		/*
		 *  Set the right motors to a specified speed.
		 *  @param s The proportion of the max speed to set the motors to, in the range [-1.0, 1.0].
		 *  @return Whether or not the motor speed was successfully set.
		 */
		bool set_right_motor_speed(double s);
		
		/*
		 *  Set all the motors to a specified speed.
		 *  @param s The proportion of the max speed to set the motors to, in the range [-1.0, 1.0].
		 *  @return Whether or not the motor speed was successfully set.
		 */
		bool set_both_motor_speeds(double s);
};

#endif /* TSIBU_THUNDERBORG_CONTROLLER_HPP */