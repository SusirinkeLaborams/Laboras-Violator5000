#include "PrecompiledHeader.h"
#include "Interface.h"

uint8_t Hash(RobotOutput e)
{
	uint8_t hash = 0;
	for (int i = 0; i < SensorCount; i++)
	{
		hash ^= e.Sensors[i];
	}
	return hash;
}