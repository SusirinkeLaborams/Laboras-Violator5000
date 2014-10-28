#include "PrecompiledHeader.h"
#include "ComPort.h"
#include "Core\Settings.h"
#include "Utilities\Utilities.h"

ComPort::ComPort() :
	m_Running(true),
	m_HasDataToRead(false),
	m_HasDataToWrite(false)
{
	m_Handle = CreateFileW(Settings::RobotConstants::kComPortName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		nullptr,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
		nullptr);

	Assert(m_Handle != INVALID_HANDLE_VALUE);

	DCB dcb;
	auto result = GetCommState(m_Handle, &dcb);
	Assert(result);

	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;

	result = SetCommState(m_Handle, &dcb);
	Assert(result);

	m_ReadEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
	Assert(m_ReadEvent != nullptr);

	m_StartTime = static_cast<float>(Utilities::GetTime());

	m_ReadingThread = std::thread([this]()
	{
		ReadWorker();
	});

	m_WritingThread = std::thread([this]()
	{
		WriteWorker();
	});
}

ComPort::~ComPort()
{
	m_Running = false;
	m_ReadingThread.join();
	m_WritingThread.join();
}

static bool ReadFileWithTimeout(HANDLE fileHandle, void* buffer, DWORD bytesToRead)
{
	OVERLAPPED overlappedReader = { 0 };
	overlappedReader.hEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);

	const int kReadTimeOutInMilliseconds = 200;
	DWORD bytesRead;

	auto result = ReadFile(fileHandle, buffer, bytesToRead, nullptr, &overlappedReader);

	if (result == FALSE)
	{
		Assert(GetLastError() == ERROR_IO_PENDING);

		if (WaitForSingleObject(overlappedReader.hEvent, kReadTimeOutInMilliseconds) != WAIT_OBJECT_0)
		{
			result = CancelIoEx(fileHandle, &overlappedReader);
		}
	}

	result = GetOverlappedResult(fileHandle, &overlappedReader, &bytesRead, TRUE);

	if (result == FALSE)
	{
		return false;
	}

	CloseHandle(overlappedReader.hEvent);

	Assert(bytesRead == bytesToRead);
	return true;
}

void ComPort::ReadWorker()
{
	RobotOutput data;

	while (m_Running)
	{
		int matchingMagicBytes = 0;

		while (matchingMagicBytes < sizeof(data.Magic))
		{
			uint64_t byte = 0;

			if (!ReadFileWithTimeout(m_Handle, &byte, 8))
			{
				continue;
			}

			if (byte == (RobotOutput::MagicBytes >> (8 * matchingMagicBytes)) % (1 << 8))
			{
				*(reinterpret_cast<uint8_t*>(&data.Magic) + matchingMagicBytes) = byte;
				matchingMagicBytes++;
			}
			else
			{
				matchingMagicBytes = 0;
			}
		}

		auto bytesToRead = sizeof(data) - sizeof(data.Magic);
		auto dataPtr = reinterpret_cast<uint8_t*>(&data) + sizeof(data.Magic);

		if (!ReadFileWithTimeout(m_Handle, dataPtr, bytesToRead))
		{
			continue;
		}

		if (data.Hash == RobotOutput::CalculateHash(data))
		{
			CriticalSection::Lock lock(m_ReadCriticalSection);
			m_HasDataToRead = true;
			m_ReadBuffer = data;
		}
	}
}

void ComPort::WriteWorker()
{
	const int kWriteTimeOutInMIlliseconds = 500;
	DWORD bytesWritten;

	while (m_Running)
	{
		if (m_HasDataToWrite)
		{
			RobotInput dataToWrite;

			{
				CriticalSection::Lock lock(m_WriteCriticalSection);
				dataToWrite = m_WriteBuffer;
			}

			OVERLAPPED overlappedWriter = { 0 };
			overlappedWriter.hEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
			auto result = WriteFile(m_Handle, &dataToWrite, sizeof(dataToWrite), nullptr, &overlappedWriter);
			Assert(result == FALSE);
			Assert(GetLastError() == ERROR_IO_PENDING);

			if (WaitForSingleObject(overlappedWriter.hEvent, kWriteTimeOutInMIlliseconds) != WAIT_OBJECT_0)
			{
				result = CancelIoEx(m_Handle, &overlappedWriter);
			}

			result = GetOverlappedResult(m_Handle, &overlappedWriter, &bytesWritten, TRUE);

			if (result != FALSE)
			{
				Assert(bytesWritten == sizeof(dataToWrite));
			}

			CloseHandle(overlappedWriter.hEvent);
			m_HasDataToWrite = false;
		}

		Sleep(static_cast<DWORD>(Settings::RobotConstants::kWriteCooldown * 1000.0f));
	}
}

bool ComPort::Read(RobotOutput& data)
{
	CriticalSection::Lock lock(m_ReadCriticalSection);

	if (!m_HasDataToRead)
	{
		return false;
	}

	data = m_ReadBuffer;
	m_HasDataToRead = false;
	return true;	
}

void ComPort::Write(const RobotInput &data)
{
	CriticalSection::Lock lock(m_WriteCriticalSection);
	m_WriteBuffer = data;
	m_HasDataToWrite = true;
}