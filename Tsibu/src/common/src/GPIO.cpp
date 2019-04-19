#include "../include/GPIO.hpp"

int16_t GPIO::get_current_value(bool is_digital)
{
	file_io_mutex.lock();
	
	std::string file_URL = "/sys/class/gpio/gpio" + std::to_string(pin_number) + "/value";
	std::ifstream file_get_value(file_URL.c_str());
	if (!file_get_value.is_open())
	{
		return -1;
	}
	
	int read_value;
	file_get_value >> read_value;
	
	if (is_digital && (read_value != 0))
	{
		read_value = 1;
	}
	
	file_get_value.close();
	wait_after_file_operation();
	file_io_mutex.unlock();
	
	return read_value;
}

GPIO::GPIO(int pn, GPIOMode im)
{
	pin_number = -1;
	
	if (GPIORegistrar::request(pn))
	{
		pin_number = pn;
	}
	else
	{
		throw std::invalid_argument(std::string("Failed to acquire GPIO pin #") + std::to_string(pin_number));
	}
	
	if (!export_pin())
	{
		throw std::invalid_argument(std::string("Failed to export GPIO pin #") + std::to_string(pin_number));
	}
	
	if ((im != GPIOMode::UNDEFINED) && (!set_mode(im)))
	{
		throw std::invalid_argument(std::string("Failed to set GPIO pin mode: ") + std::to_string(static_cast<int>(im)));
	}
}

GPIO::GPIO(int pn) : GPIO(pn, GPIOMode::UNDEFINED)
{
	// Nothing else to do
}

GPIO::~GPIO()
{
	if (GPIORegistrar::pin_is_legal(pin_number))
	{
		if (!unexport_pin())
		{
			throw std::invalid_argument(std::string("Failed to unexport GPIO pin #") + std::to_string(pin_number));
		}
		
		if (!GPIORegistrar::release(pin_number))
		{
			throw std::invalid_argument(std::string("Failed to export GPIO pin #") + std::to_string(pin_number));
		}
		
		wait_after_file_operation();
	}
}

void GPIO::wait_after_file_operation()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

GPIOMode GPIO::get_mode()
{
	return last_set_mode;
}

bool GPIO::export_pin()
{
	file_io_mutex.lock();
	
	std::ofstream file_export("/sys/class/gpio/export");
	if (!file_export.is_open())
	{
		return false;
	}
	
	file_export << pin_number;
	
	file_export.close();
	wait_after_file_operation();
	file_io_mutex.unlock();
	
	return true;
}

bool GPIO::unexport_pin()
{
	file_io_mutex.lock();
	
	std::ofstream file_unexport("/sys/class/gpio/unexport");
	if (!file_unexport.is_open())
	{
		return false;
	}
	
	file_unexport << pin_number;
	
	file_unexport.close();
	wait_after_file_operation();
	file_io_mutex.unlock();
	
	return true;
}

bool GPIO::set_mode(GPIOMode new_mode)
{
	file_io_mutex.lock();
	
	std::string file_URL = "/sys/class/gpio/gpio" + std::to_string(pin_number) + "/direction";
	std::ofstream file_mode(file_URL.c_str());
	if (!file_mode.is_open())
	{
		std::cout << "Direction file not open!" << std::endl;
		return false;
	}
	
	const char* chars_to_write = gpio_mode_to_chars(new_mode);
	if (chars_to_write == nullptr)
	{
		std::cout << "Direction chars not valid!" << std::endl;
		return false;
	}
	
	file_mode << chars_to_write;
	
	file_mode.close();
	
	last_set_mode = new_mode;
	
	wait_after_file_operation();
	file_io_mutex.unlock();
	
	return true;
}

bool GPIO::set_digital_output_value(bool value)
{
	set_analog_output_value(value ? 1 : 0);
}

bool GPIO::set_analog_output_value(uint8_t new_output_value)
{
	file_io_mutex.lock();
	
	std::string file_URL = "/sys/class/gpio/gpio" + std::to_string(pin_number) + "/value";
	std::ofstream file_set_value(file_URL.c_str());
	if (!file_set_value.is_open())
	{
		return false;
	}
	
	file_set_value << std::to_string(new_output_value);
	
	file_set_value.close();
	wait_after_file_operation();
	file_io_mutex.unlock();
	
	return true;
}

int16_t GPIO::get_current_digital_value()
{
	return get_current_value(true);
}

int16_t GPIO::get_current_analog_value()
{
	return get_current_value(false);
}