#pragma once
#ifdef ARDUINO
#include "Arduino.h"
#else
using uint8_t = unsigned char;
#endif

const uint8_t SensorCount = 2;
struct RobotInput {
public:
	uint8_t DirectionL : 1;
	uint8_t PowerL : 3;
	uint8_t DirectionR : 1;
	uint8_t PowerR : 3;
};

struct RobotOutput {
public:
    static const uint32_t MagicBytes = 2564348594;
	uint32_t Magic;
	uint32_t Sensors[SensorCount];
	uint32_t Hash;
};

inline uint32_t Hash(const RobotOutput& e)
{
	uint32_t hash = 0;
	for (int i = 0; i < SensorCount; i++)
	{
		hash ^= e.Sensors[i];
	}
	return hash;
}