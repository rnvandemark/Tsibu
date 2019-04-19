#ifndef TSIBU_GPIO_MODE_HPP
#define TSIBU_GPIO_MODE_HPP

/*
 *  This class is the means of controlling the general purpose inputs and outputs on the mini-PC that the AI operates off of.
 */
enum GPIOMode
{
	/*
	 *  Defines being in an unknown state.
	 */
	UNDEFINED = 1,
	
	/*
	 *  Defines being in an input state.
	 */
	INPUT = 2,
	
	/*
	 *  Defines being in an output state.
	 */
	OUTPUT = 3
};

inline const char* gpio_mode_to_chars(GPIOMode mode)
{
	switch(mode)
	{
		case GPIOMode::INPUT:
			return "in";
		
		case GPIOMode::OUTPUT:
			return "out";
		
		default:
			return nullptr;
	}
};

#endif /* TSIBU_GPIO_MODE_HPP */