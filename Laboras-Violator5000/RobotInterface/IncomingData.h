#pragma once

struct IncomingData
{
public:
	static const int kSensorCount = 2;

	DirectX::XMFLOAT2 robotPosition;
	float robotRotation;
	DirectX::XMFLOAT2 data[kSensorCount];
};
