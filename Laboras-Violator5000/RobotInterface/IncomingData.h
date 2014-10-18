#pragma once

struct IncomingData
{
public:
	static const int kSensorCount = 4;

	DirectX::XMFLOAT2 robotPosition;
	DirectX::XMFLOAT2 robotDirection;
	DirectX::XMFLOAT2 data[kSensorCount];
};
