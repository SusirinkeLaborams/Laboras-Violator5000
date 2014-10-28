#pragma once

#ifdef ARDUINO
#include "Arduino.h"
#endif

const uint8_t kSensorCount = 2;

struct RobotInput {
public:
	uint8_t DirectionL : 1;
	uint8_t PowerL : 3;
	uint8_t DirectionR : 1;
	uint8_t PowerR : 3;
};

struct RobotOutput {
public:
    static const uint8_t MagicByte = 255;
	uint8_t Magic;
	uint8_t Hash;
	uint32_t Sensors[kSensorCount];
};

struct IncomingData
{
public:
	DirectX::XMFLOAT2 robotPosition;
	float robotRotation;
	DirectX::XMFLOAT2 data[kSensorCount];
};