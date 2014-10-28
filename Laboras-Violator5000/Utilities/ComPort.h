#pragma once

#include "CriticalSection.h"

class ComPort
{
private:
	HANDLE m_Handle;
	float m_StartTime;
	HANDLE m_ReadEvent;

	bool m_HasDataToRead;
	RobotOutput m_ReadBuffer;

	bool m_HasDataToWrite;
	RobotInput m_WriteBuffer;

	CriticalSection m_ReadCriticalSection;
	CriticalSection m_WriteCriticalSection;

	std::thread m_ReadingThread;
	std::thread m_WritingThread;
	volatile bool m_Running;

	void ReadWorker();
	void WriteWorker();


public:
	ComPort();
	~ComPort();

	void Write(const RobotInput& data);
	bool Read(RobotOutput& data);
};