#ifndef TSIBU_GPIO_REGISTRAR_HPP
#define TSIBU_GPIO_REGISTRAR_HPP

#include <unordered_map>
#include <mutex>

#include "GPIO.hpp"

/*
 *  This static class monitors all instances of GPIO in the AI's program to ensure that pins on the board aren't being controlled by multiple sources by accident.
 */
class GPIORegistrar
{
	private:
		
		/*
		 *  A helper function to determine whether or not one of the static maps contains a specified pin number.
		 */
		template <typename V>
		static bool map_contains(unsigned int pin_number, std::unordered_map<unsigned int, V> map)
		{
			return map.find(pin_number) != map.end();
		}
		
		/*
		 *  A mutex that helps to ensure that race conditions don't interfere with the safe acquisition or release of pins.
		 */
		static std::mutex gpio_mutex;
		
		/*
		 *  The map that contains all of the system legal pins to operate on, as well as the status of each pin, where the key is the pin number and the value is
		 *  the amount of times the registrar has been informed that an element in the system will be accessing this pin throughout the lifetime of its routine.
		 *  When the routine ends, this value decrements.
		 */
		static std::unordered_map<unsigned int, volatile unsigned int> pin_statuses;
		
		/*
		 *  Pointers to each GPIO object allocated for some element of the program. For there to be an instance of the GPIO in this map, there must be at least one
		 *  reference to it elsewhere in the program already (if the value of pin_statuses[pin_number] > 0). Otherwise, it should be allocated.
		 */
		static std::unordered_map<unsigned int, GPIO*> pin_objects;
		
		/*
		 *  Restrict the default and only constructor to be private, so no instances can be made.
		 */
		GPIORegistrar();
	
	public:
	
		/*
		 *  A function to check if the pin number is considered legal to operate with.
		 *  @param pin_number The pin index of interest.
		 *  @return Whether or not the pin has been added as a legal pin.
		 */
		static bool pin_is_legal(unsigned int pin_number);
	
		/*
		 *  A function to add a pin number to those that can be used.
		 *  @param pin_number The pin index of interest.
		 *  @return Whether or not the pin was added.
		 */
		static bool add_legal_pin(unsigned int pin_number);
	
		/*
		 *  A function to remove a pin number from those that can be used.
		 *  @param pin_number The pin index of interest.
		 *  @return Whether or not the pin was removed.
		 */
		static bool remove_legal_pin(unsigned int pin_number);
	
		/*
		 *  A function to request ownership of a pin at a specific index.
		 *  @param pin_number The pin index of interest.
		 *  @return Whether or not the pin was acquired.
		 */
		static GPIO* request(unsigned int pin_number);
	
		/*
		 *  A function to release ownership of a pin at a specific index.
		 *  @param pin_number The pin index of interest.
		 *  @return Whether or not the pin was released.
		 */
		static bool release(unsigned int pin_number);
};

#endif /* TSIBU_GPIO_REGISTRAR_HPP */