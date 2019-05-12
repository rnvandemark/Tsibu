#ifndef TSIBU_SERIAL_CONSTANTS_HPP
#define TSIBU_SERIAL_CONSTANTS_HPP

#define SERIAL_START_RXTX_READ_REQUEST static_cast<unsigned char>('~')
#define SERIAL_START_RXTX_WRITE_REQUEST static_cast<unsigned char>('<')
#define SERIAL_START_RXTX_RESPONSE static_cast<unsigned char>('>')
#define SERIAL_START_RXTX_DESCRIPTOR static_cast<unsigned char>('!')
#define SERIAL_END_RXTX_DESCRIPTOR static_cast<unsigned char>('@')
#define SERIAL_END_RXTX static_cast<unsigned char>('#')

#define SERIAL_DIGITAL_VALUE_HIGH static_cast<unsigned char>('H')
#define SERIAL_DIGITAL_VALUE_LOW static_cast<unsigned char>('L')

#define SERIAL_WAIT_AFTER_FILE_OPEN_MICROSECONDS 5000000

#define SERIAL_READ_TIMEOUT_HUNDREDS_OF_MILLISECONDS 3

#define SERIAL_READ_OPERATION_BUFFER_SIZE 16

#define SERIAL_MAX_ANALOG_VALUE 1023

#define SERIAL_PURGE_BUFFER_SIZE 512

#endif /* TSIBU_SERIAL_CONSTANTS_HPP */