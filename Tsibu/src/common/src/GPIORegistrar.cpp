#include "../include/GPIORegistrar.hpp"

std::mutex GPIORegistrar::gpio_mutex;
std::unordered_map<unsigned int, volatile unsigned int> GPIORegistrar::pin_statuses;
std::unordered_map<unsigned int, GPIO*> GPIORegistrar::pin_objects;

GPIORegistrar::GPIORegistrar()
{
	// Does nothing interesting
}

bool GPIORegistrar::pin_is_legal(unsigned int pin_number)
{
	return map_contains<volatile unsigned int>(pin_number, GPIORegistrar::pin_statuses);
}

bool GPIORegistrar::add_legal_pin(unsigned int pin_number)
{
	GPIORegistrar::gpio_mutex.lock();
	
	if (GPIORegistrar::pin_is_legal(pin_number))
	{
		GPIORegistrar::gpio_mutex.unlock();
		return false;
	}
	
	GPIORegistrar::pin_statuses[pin_number] = 0;
	
	GPIORegistrar::gpio_mutex.unlock();
	return true;
}

bool GPIORegistrar::remove_legal_pin(unsigned int pin_number)
{
	GPIORegistrar::gpio_mutex.lock();
	
	if ((!GPIORegistrar::pin_is_legal(pin_number)) || (GPIORegistrar::pin_statuses[pin_number] != 0))
	{
		GPIORegistrar::gpio_mutex.unlock();
		return false;
	}
	
	GPIORegistrar::pin_statuses.erase(pin_number);
	
	GPIORegistrar::gpio_mutex.unlock();
	return true;
}

GPIO* GPIORegistrar::request(unsigned int pin_number)
{
	GPIORegistrar::gpio_mutex.lock();
	
	if (!GPIORegistrar::pin_is_legal(pin_number))
	{
		GPIORegistrar::gpio_mutex.unlock();
		return nullptr;
	}
	
	GPIO* instance = nullptr;
	if (GPIORegistrar::pin_statuses[pin_number] == 0)
	{
		instance = new GPIO(pin_number);
		GPIORegistrar::pin_objects[pin_number] = instance;
	}
	else
	{
		instance = GPIORegistrar::pin_objects[pin_number];
	}
	
	GPIORegistrar::pin_statuses[pin_number]++;
	
	GPIORegistrar::gpio_mutex.unlock();
	return instance;
}

bool GPIORegistrar::release(unsigned int pin_number)
{
	GPIORegistrar::gpio_mutex.lock();
	
	if ((!GPIORegistrar::pin_is_legal(pin_number)) || (GPIORegistrar::pin_statuses[pin_number] == 0))
	{
		GPIORegistrar::gpio_mutex.unlock();
		return false;
	}
	
	GPIORegistrar::pin_statuses[pin_number]--;
	if (GPIORegistrar::pin_statuses[pin_number] == 0)
	{
		delete GPIORegistrar::pin_objects[pin_number];
		GPIORegistrar::pin_objects.erase(pin_number);
	}
	
	GPIORegistrar::gpio_mutex.unlock();
	return true;
}