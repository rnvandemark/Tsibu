#ifndef TSIBU_GPIO_HPP
#define TSIBU_GPIO_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <exception>

#include "GPIOMode.hpp"
#include "GPIORegistrar.hpp"

/*
 *  This class is the means of controlling the general purpose inputs and outputs on the
 *  mini-PC that the AI operates off of. This is done by reading and writing the files
 *  associated with each pin.
 */
class GPIO
{
	private:
		
		/*
		 *  The pin that this instance owns.
		 */
		int pin_number;
		
		/*
		 *  Get the value of a generic pin type.
		 *  @param is_digital Whether or not this pin is digital, true meaning it's digital, false meaning it's analog.
		 *  @return The value that this pin currently has, -1 on failure.
		 */
		int16_t get_current_value(bool is_digital);
	
	public:
	
		/*
		 *  A constructor to initialize a pin number to an unspecified mode.
		 *  @param pn The pin number of this GPIO.
		 *  @param im The initial mode for this pin to have.
		 */
		GPIO(int pn, GPIOMode im);
		
		/*
		 *  The destructor.
		 *  Ensures that any resting state is set and unregisters itself with the global GPIO registrar.
		 */
		~GPIO();
		
		/*
		 *  Export the GPIO with this pin number so it can be used.
		 *  @return Whether or not the pin index was successfully exported.
		 */
		bool export_pin();
		
		/*
		 *  Unexport the GPIO with this pin number.
		 *  @return Whether or not the pin index was successfully unexported.
		 */
		bool unexport_pin();
		
		/*
		 *  Set the operational direction/mode of the GPIO (input or output).
		 *  @param new_mode The desired mode for this pin.
		 *  @return Whether or not the mode was successfully set.
		 */
		bool set_mode(GPIOMode new_mode);
		
		/*
		 *  Set the value of a digital output pin.
		 *  @param value The desired value for this digital pin, where false is LOW and true is HIGH
		 *  @return Whether or not the value was successfully set.
		 */
		bool set_digital_output_value(bool value);
		
		/*
		 *  Set the value of an analog output pin.
		 *  @param new_output_value The desired value for this analog pin.
		 *  @return Whether or not the value was successfully set.
		 */
		bool set_analog_output_value(uint8_t new_output_value);
		
		/*
		 *  Get the value of a digital pin.
		 *  @return The value that this pin currently has, -1 on failure.
		 */
		int16_t get_current_digital_value();
		
		/*
		 *  Get the value of an analog pin.
		 *  @return The value that this pin currently has, -1 on failure.
		 */
		int16_t get_current_analog_value();
};

#endif /* TSIBU_GPIO_HPP */