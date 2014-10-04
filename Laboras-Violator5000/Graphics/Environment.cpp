#include "PrecompiledHeader.h"
#include "Core\Settings.h"
#include "Environment.h"
#include "Utilities\Utilities.h"

Environment::Environment() :
	m_DataBuffer(new DataStorage[Settings::kPathTracingBufferSize]),
	m_BufferBegin(0),
	m_BufferEnd(0)
{
}

Environment::~Environment()
{
}

void Environment::AddData(const DirectX::XMFLOAT2(&data)[IncomingData::kSensorCount])
{
	for (const auto& dataItem : data)
	{
		m_DataBuffer[m_BufferEnd].position = dataItem;
		m_DataBuffer[m_BufferEnd].timestamp = Utilities::GetTime();

		m_BufferEnd = (m_BufferEnd + 1) % Settings::kPathTracingBufferSize;
	}
}