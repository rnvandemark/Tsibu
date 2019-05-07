#ifndef TSIBU_MOVEMENT_DIRECTION
#define TSIBU_MOVEMENT_DIRECTION

/*
 *  The objective direction for the AI to be travelling.
 */
enum MovementDirection
{
	/*
	 *  The AI should be stationary.
	 */
	NONE = 1,

	/*
	 *  The AI should be moving forward.
	 */
	FORWARD = 2,

	/*
	 *  The AI should be moving in reverse.
	 */
	REVERSE = 3,

	/*
	 *  The AI should be turning on the spot in a clockwise direction.
	 */
	RADIAL_CW = 4,

	/*
	 *  The AI should be turning on the spot in a counter-clockwise direction.
	 */
	RADIAL_CCW = 5
};

#endif /* TSIBU_MOVEMENT_DIRECTION */
