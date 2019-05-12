#ifndef TSIBU_MOTOR_TYPE
#define TSIBU_MOTOR_TYPE

/*
 *  A simple definition to help specify what motor a motor command applies to.
 */
enum MotorType
{
	/*
	 *  The left three motors of the AI.
	 */
	LEFT = 1,

	/*
	 *  The right three motors of the AI.
	 */
	RIGHT = 2,

	/*
	 *  All six motors of the AI.
	 */
	BOTH = 3
};

#endif /* TSIBU_MOTOR_TYPE */