#pragma once
#include "PrecompiledHeader.h"
class ComPort
{
	HANDLE handle;
	bool open;
	std::string name;
public:
	void Open();
	void Close();
	bool IsOpen(){ return open; }

	template<typename T>
	T Read();

	template<typename T>
	void Write(const T &data);

	ComPort(std::string name);
	~ComPort();
};

template<typename T>
T ComPort::Read()
{
	//fail if closed
	Assert(open);

	T ret;
	DWORD dwRead;
	OVERLAPPED reader = { 0 };
	reader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	Assert(reader.hEvent != NULL);

	if (ReadFile(handle, &ret, sizeof(T), &dwRead, &reader))
	{
		//fail if read wrong amount of stuff
		Assert(dwRead == sizeof(T));
	}
	else
	{
		Assert(GetLastError() == ERROR_IO_PENDING);

		switch (WaitForSingleObject(reader, 2000))
		{
		case WAIT_OBJECT_0:
			Assert(GetOverlappedResult(handle, &reader, &dwRead, FALSE));
			Assert(dwRead == sizeof(T));
		case WAIT_TIMEOUT:
		default:
			Assert(false);
			break;
		}
	}

	CloseHandle(reader.hEvent);
	return ret;
}

template<typename T>
void ComPort::Write(const T &data)
{
	OVERLAPPED writer = { 0 };
	DWORD written;
	writer.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	Assert(writer.hEvent != NULL);

	if (!WriteFile(handle, &data, sizeof(T), &written, &writer))
	{
		Assert(GetLastError() == ERROR_IO_PENDING);
		SetLastError(ERROR_SUCCESS);//fuck winapi
		auto waitResult = WaitForSingleObject(writer, INFINITE);
		Assert(waitResult == WAIT_OBJECT_0);
		auto overlappedResult = GetOverlappedResult(handle, writer, &written, FALSE);
		Assert(overlappedResult);
		Assert(written == sizeof(T));
	}

	CloseHandle(writer.hEvent);
}