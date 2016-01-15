#pragma once
#include <iostream>
#include <ctime>
#include "Utility.h"

void rand_init()
{
	std::srand(time(0));
}

// Returns random float between [min, value]
float rand(float value, float min)
{
	return (std::rand() * value - min)/ RAND_MAX + min;
}

