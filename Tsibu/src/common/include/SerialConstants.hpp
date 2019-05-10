#ifndef TSIBU_SERIAL_CONSTANTS_HPP
#define TSIBU_SERIAL_CONSTANTS_HPP

#define START_RXTX_READ_REQUEST static_cast<unsigned char>('~')
#define START_RXTX_WRITE_REQUEST static_cast<unsigned char>('<')
#define START_RXTX_RESPONSE static_cast<unsigned char>('>')
#define START_RXTX_DESCRIPTOR static_cast<unsigned char>('!')
#define END_RXTX_DESCRIPTOR static_cast<unsigned char>('@')
#define END_RXTX static_cast<unsigned char>('#')

#define DIGITAL_VALUE_HIGH static_cast<unsigned char>('H')
#define DIGITAL_VALUE_LOW static_cast<unsigned char>('L')

#define WAIT_AFTER_FILE_OPEN_MICROSECONDS 5000000

#define READ_TIMEOUT_HUNDREDS_OF_MILLISECONDS 3

#define READ_OPERATION_BUFFER_SIZE 16

#define MAX_ANALOG_VALUE 1023

#define PURGE_BUFFER_SIZE 512

#endif /* TSIBU_SERIAL_CONSTANTS_HPP */