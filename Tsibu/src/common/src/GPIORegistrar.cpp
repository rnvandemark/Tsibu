#include "../include/GPIORegistrar.hpp"

std::mutex GPIORegistrar::gpio_mutex;
std::unordered_map<int, volatile unsigned int> GPIORegistrar::pin_statuses;
std::unordered_map<int, GPIO*> GPIORegistrar::pin_objects;

GPIORegistrar::GPIORegistrar()
{
	// Does nothing interesting
}

bool GPIORegistrar::pin_is_legal(int pin_number)
{
	return map_contains<volatile unsigned int>(pin_number, GPIORegistrar::pin_statuses);
}

bool GPIORegistrar::add_legal_pin(int pin_number)
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

bool GPIORegistrar::remove_legal_pin(int pin_number)
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

GPIO* GPIORegistrar::request(int pin_number)
{
	GPIORegistrar::gpio_mutex.lock();
	
	if (!GPIORegistrar::pin_is_legal(pin_number))
	{
		GPIORegistrar::gpio_mutex.unlock();
		return nullptr;
	}
	
	if (GPIORegistrar::pin_statuses[pin_number] == 0)
	{
		GPIORegistrar::pin_objects[pin_number] = GPIOHelper::factory(pin_number);
	}
	
	GPIO* instance = GPIORegistrar::pin_objects[pin_number];
	GPIORegistrar::pin_statuses[pin_number]++;
	
	GPIORegistrar::gpio_mutex.unlock();
	
	return instance;
}

bool GPIORegistrar::release(int pin_number)
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