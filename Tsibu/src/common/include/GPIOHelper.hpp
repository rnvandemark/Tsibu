#ifndef TSIBU_GPIO_HELPER_HPP
#define TSIBU_GPIO_HELPER_HPP

#include "GPIO.hpp"

/*
 *  A very simple namespace that was made to counteract the problem of circular dependencies. I really hate them sometimes.
 */
namespace GPIOHelper
{
	/*
	 *  A factory function to create pointers to GPIO instances.
	 *  @param pn The pin number of this GPIO.
	 *  @param im The initial mode for this pin to have.
	 *  @return A new dynamically allocated instance.
	 */
	inline GPIO* factory(int pn)
	{
		return new GPIO(pn);
	}
}

#endif /* TSIBU_GPIO_HELPER_HPP */