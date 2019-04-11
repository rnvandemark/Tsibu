#ifndef TSIBU_GPIO_REGISTRAR_HPP
#define TSIBU_GPIO_REGISTRAR_HPP

#include <unordered_map>
#include <mutex>

/*
 *  This static class monitors all instances of GPIO in the AI's program to ensure that pins on the board
 *  aren't being controlled by multiple sources by accident.
 */
class GPIORegistrar
{
	private:
		
		/*
		 *  A mutex that helps to ensure that race conditions don't interfere with the safe acquisition or release of pins.
		 */
		static std::mutex gpio_mutex;
		
		/*
		 *  The map that tracks the status of each pin, where the key is the pin number and the boolean is whether or not it's occupied somewhere in the program.
		 */
		static std::unordered_map<int, bool> pin_statuses;
		
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
		static bool pin_is_legal(int pin_number);
	
		/*
		 *  A function to add a pin number to those that can be used.
		 *  @param pin_number The pin index of interest.
		 *  @return Whether or not the pin was added.
		 */
		static bool add_legal_pin(int pin_number);
	
		/*
		 *  A function to remove a pin number from those that can be used.
		 *  @param pin_number The pin index of interest.
		 *  @return Whether or not the pin was removed.
		 */
		static bool remove_legal_pin(int pin_number);
	
		/*
		 *  A function to request ownership of a pin at a specific index.
		 *  @param pin_number The pin index of interest.
		 *  @return Whether or not the pin was acquired.
		 */
		static bool request(int pin_number);
	
		/*
		 *  A function to release ownership of a pin at a specific index.
		 *  @param pin_number The pin index of interest.
		 *  @return Whether or not the pin was released.
		 */
		static bool release(int pin_number);
};

#endif /* TSIBU_GPIO_REGISTRAR_HPP */