#pragma once

struct IncomingData
{
public:
	static const int kSensorCount = 8;

	DirectX::XMFLOAT2 robotPosition;
	DirectX::XMFLOAT2 data[kSensorCount];
};
