#include "sensors.h"
#include <iostream>

Sensors::Sensors()
{
	this->sens.gyr = { 0,0,0 };
	this->sens.acc = { 0,0,0 };
	this->sens.mag = { 0,0,0 };
	this->sens.baro_pressure = 0;
}

Sensors::~Sensors()
{
}

void Sensors::sensor_print()
{
	std::cout << this->sens.gyr[0] << ", " << std::endl;
	std::cout << this->sens.gyr[1] << ", " << std::endl;
	std::cout << this->sens.gyr[2] << ", " << std::endl;
	std::cout << this->sens.acc[0] << ", " << std::endl;
	std::cout << this->sens.acc[1] << ", " << std::endl;
	std::cout << this->sens.acc[2] << ", " << std::endl;
	std::cout << this->sens.mag[0] << ", " << std::endl;
	std::cout << this->sens.mag[1] << ", " << std::endl;
	std::cout << this->sens.mag[2] << ", " << std::endl;
	std::cout << this->sens.baro_pressure << std::endl;

}
