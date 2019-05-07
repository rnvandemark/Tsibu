#ifndef TSIBU_OPERATION_MODE
#define TSIBU_OPERATION_MODE

/*
 *  The objective activity for the AI to perform.
 */
enum OperationMode
{
	/*
	 *  The AI does not have any objective.
	 */
	IDLE = 1,

	/*
	 *  The AI should be exploring its surroundings randomly.
	 */
	EXPLORING = 2,

	/*
	 *  The AI should be trying to find food.
	 */
	FINDING_FOOD = 3,

	/*
	 *  The AI should be playing with elements of its environment.
	 */
	PLAYING = 4,

	/*
	 *  The AI should be fearing elements of its environment.
	 */
	FEARING = 5
};

#endif /* TSIBU_OPERATION_MODE */