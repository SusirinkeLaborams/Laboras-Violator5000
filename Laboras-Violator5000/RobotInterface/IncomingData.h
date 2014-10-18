#pragma once

struct IncomingData
{
public:
	static const int kSensorCount = 4;

	DirectX::XMFLOAT2 robotPosition;
	float robotRotation;
	DirectX::XMFLOAT2 data[kSensorCount];
};
