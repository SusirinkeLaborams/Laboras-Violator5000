#include "PrecompiledHeader.h"
#include "ComPort.h"

ComPort::ComPort(std::string name)
	:name(name)
{
	Open();
}


ComPort::~ComPort()
{
	Close();
}

void ComPort::Open()
{
	handle = CreateFileA(
		name.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING,
		NULL);

	open = (handle != INVALID_HANDLE_VALUE);

	if (open)
	{
		DCB dcb;
		ZeroMemory(&dcb, sizeof(dcb));
		dcb.BaudRate = CBR_9600;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;
		dcb.Parity = NOPARITY;
		SetCommState(handle, &dcb);
	}
}

void ComPort::Close()
{
	if (open)
	{
		CloseHandle(handle);
		open = false;
	}
}

RobotOutput ComPort::Read()
{
	//fail if closed
	Assert(open);

	RobotOutput data;

	int bytesRead = 0;
	DWORD dwRead;

	BOOL result;

	while (true)
	{
		int matchingMagicBytes = 0;

		while (matchingMagicBytes < sizeof(data.Magic))
		{
			uint8_t byte;

			result = ReadFile(handle, &byte, sizeof(byte), &dwRead, NULL);
			Assert(result);
			Assert(dwRead == sizeof(byte));

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

		result = ReadFile(handle, dataPtr, bytesToRead, &dwRead, NULL);
		Assert(result);
		Assert(dwRead == bytesToRead);

		if (data.Hash == Hash(data))
			break;
	}

	return data;
}

void ComPort::Write(const RobotInput &data)
{
	Assert(open);

	DWORD written;

	auto result = WriteFile(handle, &data, sizeof(data), &written, NULL);
	Assert(result);
	Assert(written == sizeof(data));
}