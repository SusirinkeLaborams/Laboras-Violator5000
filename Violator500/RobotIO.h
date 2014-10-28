#pragma once

#ifdef ARDUINO
#include "Arduino.h"
#endif

const uint8_t kSensorCount = 2;

struct RobotInput 
{
public:
	uint8_t DirectionL : 1;
	uint8_t PowerL : 3;
	uint8_t DirectionR : 1;
	uint8_t PowerR : 3;
};

struct RobotOutput 
{
public:
	static const uint32_t MagicBytes = 2564348594;
	uint32_t Magic;
	uint32_t Hash;
	uint32_t Sensors[kSensorCount];

	static inline uint32_t CalculateHash(const RobotOutput& e)
	{
		uint32_t hash = 0;
		for (int i = 0; i < kSensorCount; i++)
		{
			hash ^= e.Sensors[i];
		}
		return hash;
	}
};

struct IncomingData
{
public:
	DirectX::XMFLOAT2 robotPosition;
	float robotRotation;
	DirectX::XMFLOAT2 data[kSensorCount];
};
