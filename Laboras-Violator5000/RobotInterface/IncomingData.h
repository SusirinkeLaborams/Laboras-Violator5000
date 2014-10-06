#pragma once

namespace DX
{
	using namespace DirectX;
}

struct IncomingData
{
public:
	static const int kSensorCount = 4;

	DX::XMFLOAT2 robotPosition;
	DX::XMFLOAT2 data[kSensorCount];
};
