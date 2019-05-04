#ifndef TSIBU_BAUD_RATE_HPP
#define TSIBU_BAUD_RATE_HPP

#include <termios.h>

/*
 *  This enum is to clarify the applicable baud rates for the serial communication.
 */
enum BaudRate
{
	/*
	 *  1200 bits per second.
	 */
	_1200 = B1200,
	
	/*
	 *  2400 bits per second.
	 */
	_2400 = B2400,
	
	/*
	 *  4800 bits per second.
	 */
	_4800 = B4800,
	
	/*
	 *  9600 bits per second.
	 */
	_9600 = B9600,
	
	/*
	 *  19200 bits per second.
	 */
	_19200 = B19200,
	
	/*
	 *  38400 bits per second.
	 */
	_38400 = B38400,
	
	/*
	 *  57600 bits per second.
	 */
	_57600 = B57600,
	
	/*
	 *  115200 bits per second.
	 */
	_115200 = B115200,
	
	/*
	 *  230400 bits per second.
	 */
	_230400 = B230400
};

#endif /* TSIBU_BAUD_RATE_HPP */