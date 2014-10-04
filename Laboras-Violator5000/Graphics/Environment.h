#pragma once

#include "RobotInterface\IncomingData.h"

class Environment
{
	struct DataStorage
	{
		DirectX::XMFLOAT2 position;
		double timestamp;
	};

	std::unique_ptr<DataStorage[]> m_DataBuffer;
	size_t m_BufferBegin;
	size_t m_BufferEnd;

public:
	Environment();
	~Environment();

	void AddData(const DirectX::XMFLOAT2 (&data)[IncomingData::kSensorCount]);
};