#ifndef TSIBU_SURROUNDINGS_ANALYSIS_HPP
#define TSIBU_SURROUNDINGS_ANALYSIS_HPP

/*
 *  This describes the AI's awareness of its objective surroundings.
 */
enum SurroundingsAnalysis
{
	/*
	 *  The AI is not threatened by anything being or getting too close.
	 */
	NOTHING_NOTICEABLE = 1,

	/*
	 *  The AI is starting to notice something it's moving closer to a non-stationary object.
	 */
	STATIC_OBJECT_GETTING_TOO_CLOSE = 2,

	/*
	 *  The AI is noticing that it's moved too close to an object to get closer.
	 */
	STATIC_OBJECT_IS_TOO_CLOSE = 3,

	/*
	 *  The AI is starting to notice something is moving closer to it.
	 */
	KINETIC_OBJECT_GETTING_TOO_CLOSE = 4,

	/*
	 *  The AI is noticing that a moving object has gotten too close to it.
	 */
	KINETIC_OBJECT_IS_TOO_CLOSE = 5
};

#endif /* TSIBU_SURROUNDINGS_ANALYSIS_HPP */
