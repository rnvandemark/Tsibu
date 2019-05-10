#ifndef TSIBU_THUNDERBORG_CONTROLLER_HPP
#define TSIBU_THUNDERBORG_CONTROLLER_HPP

#include "../../common/include/I2CCommunicator.hpp"

#include "ThunderBorgConstants.hpp"

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

	public:

		/*
		 *  The constructor.
		 *  Instantiates the I2C communicator.
		 */
		explicit ThunderBorgController();
};

#endif /* TSIBU_THUNDERBORG_CONTROLLER_HPP */