#include "PrecompiledHeader.h"
#include "ComPort.h"

ComPort::ComPort(std::string name)
	:name(name)
{
	Open();
	bytesToRead = sizeof(uint8_t) * SensorCount;
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
		0,
		OPEN_EXISTING,
		0,
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

	byte buff;
	BOOL result;

	while (true)
	{
		while (true)
		{
			result = ReadFile(handle, &buff, sizeof(byte), &dwRead, NULL);
			Assert(result);
			Assert(dwRead == sizeof(byte));
			if (buff == RobotOutput::MagicByte)
				break;
		}

		result = ReadFile(handle, &data.Hash, sizeof(byte), &dwRead, NULL);
		Assert(result);
		Assert(dwRead == sizeof(byte));

		for (int i = 0; i < bytesToRead; i++)
		{
			result = ReadFile(handle, &data.Sensors + i, sizeof(byte), &dwRead, NULL);
			Assert(result);
			Assert(dwRead == sizeof(byte));
		}

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