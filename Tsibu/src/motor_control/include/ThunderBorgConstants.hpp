#ifndef TSIBU_THUNDERBORG_CONSTANTS_HPP
#define TSIBU_THUNDERBORG_CONSTANTS_HPP

#define THUNDERBORG_I2C_BUS_URL_DEFAULT "/dev/i2c-1"
#define THUNDERBORG_I2C_DEVICE_ID_DEFAULT 0x15

#define THUNDERBORG_PWM_MAX static_cast<unsigned char>(255)

#define THUNDERBORG_CMD_VALUE_ON static_cast<unsigned char>(1)
#define THUNDERBORG_CMD_VALUE_OFF static_cast<unsigned char>(0)

#define THUNDERBORG_CMD_VALUE_FORWARD static_cast<unsigned char>(1)
#define THUNDERBORG_CMD_VALUE_REVERSE static_cast<unsigned char>(2)

#define THUNDERBORG_CMD_SET_LED static_cast<unsigned char>(1)
#define THUNDERBORG_CMD_GET_LED static_cast<unsigned char>(2)

#define THUNDERBORG_CMD_SET_LEFT_MOTOR_FORWARD  static_cast<unsigned char>(8)
#define THUNDERBORG_CMD_SET_LEFT_MOTOR_REVERSE  static_cast<unsigned char>(9)
#define THUNDERBORG_CMD_SET_RIGHT_MOTOR_FORWARD static_cast<unsigned char>(11)
#define THUNDERBORG_CMD_SET_RIGHT_MOTOR_REVERSE static_cast<unsigned char>(12)
#define THUNDERBORG_CMD_SET_BOTH_MOTORS_FORWARD static_cast<unsigned char>(17)
#define THUNDERBORG_CMD_SET_BOTH_MOTORS_REVERSE static_cast<unsigned char>(18)

#define THUNDERBORG_CMD_SET_MOTOR_FAILSAFE static_cast<unsigned char>(19)

#define THUNDERBORG_MOTOR_SPEED_MAGNITUDE_DEFAULT 0.4

#endif /* TSIBU_THUNDERBORG_CONSTANTS_HPP */
