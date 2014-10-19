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
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0);
	open = (handle != INVALID_HANDLE_VALUE);
}

void ComPort::Close()
{
	if (open)
	{
		CloseHandle(handle);
		open = false;
	}
}