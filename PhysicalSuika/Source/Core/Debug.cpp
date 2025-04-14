#include "pch.h"
#include "Debug.h"
#include <iostream>


void Log::Log(const std::string& Message)
{
	std::cerr << Message << std::endl;
}
