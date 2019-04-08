#ifndef TSIBU_HUNGER_LEVEL_HPP
#define TSIBU_HUNGER_LEVEL_HPP

/*
 *  This describes the magnitude of hunger the AI is experiencing.
 */
enum HungerLevel
{
	/*
	 *  The AI is full and not concerned about hunger.
	 */
	FULL = 1,

	/*
	 *  The AI is a little hungry.
	 */
	SOMEWHAT_HUNGRY = 2,

	/*
	 *  The AI is becoming noticeably hungry.
	 */
	MODERATELY_HUNGRY = 3,

	/*
	 *  The AI is preoccupied with hunger.
	 */
	VERY_HUNGRY = 4,

	/*
	 *  The AI needs to satisfy its hunger ASAP.
	 */
	FAMISHED = 5
};

#endif /* TSIBU_HUNGER_LEVEL_HPP */
