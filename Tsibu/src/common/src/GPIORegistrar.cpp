#include "../include/GPIORegistrar.hpp"

std::mutex GPIORegistrar::gpio_mutex;
std::unordered_map<int, bool> GPIORegistrar::pin_statuses;

GPIORegistrar::GPIORegistrar()
{
	// Does nothing interesting
}

bool GPIORegistrar::pin_is_legal(int pin_number)
{
	return GPIORegistrar::pin_statuses.find(pin_number) != GPIORegistrar::pin_statuses.end();
}

bool GPIORegistrar::add_legal_pin(int pin_number)
{
	GPIORegistrar::gpio_mutex.lock();
	
	if (GPIORegistrar::pin_is_legal(pin_number))
	{
		return false;
	}
	
	GPIORegistrar::pin_statuses[pin_number] = false;
	
	GPIORegistrar::gpio_mutex.unlock();
	
	return true;
}

bool GPIORegistrar::remove_legal_pin(int pin_number)
{
	GPIORegistrar::gpio_mutex.lock();
	
	if ((!GPIORegistrar::pin_is_legal(pin_number)) || GPIORegistrar::pin_statuses[pin_number])
	{
		return false;
	}
	
	GPIORegistrar::pin_statuses.erase(pin_number);
	
	GPIORegistrar::gpio_mutex.unlock();
	
	return true;
}

bool GPIORegistrar::request(int pin_number)
{
	GPIORegistrar::gpio_mutex.lock();
	
	if ((!GPIORegistrar::pin_is_legal(pin_number)) || GPIORegistrar::pin_statuses[pin_number])
	{
		return false;
	}
	
	GPIORegistrar::pin_statuses[pin_number] = true;
	
	GPIORegistrar::gpio_mutex.unlock();
	
	return true;
}

bool GPIORegistrar::release(int pin_number)
{
	GPIORegistrar::gpio_mutex.lock();
	
	if ((!GPIORegistrar::pin_is_legal(pin_number)) || (!GPIORegistrar::pin_statuses[pin_number]))
	{
		return false;
	}
	
	GPIORegistrar::pin_statuses[pin_number] = false;
	
	GPIORegistrar::gpio_mutex.unlock();
	
	return true;
}