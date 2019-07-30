#ifndef TSIBU_DEPTH_SENSOR_DATA_POINT_HPP
#define TSIBU_DEPTH_SENSOR_DATA_POINT_HPP

#include <chrono>

/*
 *  This struct represents a data point for a depth sensor's reading.
 */
struct DepthSensorDataPoint
{
	/*
	 *  The absolute time the reading was taken.
	 */
	std::chrono::system_clock::time_point time;
	
	/*
	 *  The raw digital value read.
	 */
	unsigned int value;
	
	/*
	 *  The constructor.
	 *  Populates the values.
	 *  @param t The absolute time the reading was taken.
	 *  @param v The raw digital value read.
	 */
	explicit DepthSensorDataPoint(std::chrono::system_clock::time_point t, unsigned int v)
	{
		time = t;
		value = v;
	}
	
	/*
	 *  Checks whether or not this time point should be considered "expired".
	 *  @param time_now A time point describing "now".
	 *  @param allowable_ms The number of miliseconds that this data point can be considered valid for.
	 *  @return Whether or not this data point is too old to be relevant.
	 */
	bool is_too_old(std::chrono::system_clock::time_point time_now, unsigned int allowable_ms)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			time_now - time
		).count() > allowable_ms;
	}
};

#endif /* TSIBU_DEPTH_SENSOR_DATA_POINT_HPP */
