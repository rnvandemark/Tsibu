#include "../include/ThunderBorgController.hpp"

ThunderBorgController::ThunderBorgController()
{
	i2c_communicator = new I2CCommunicator(THUNDERBORG_I2C_DEVICE_ID_DEFAULT);
}