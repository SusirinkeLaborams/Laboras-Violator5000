#pragma once

class ComPort
{
private:
	HANDLE handle;
	bool open;

	void Open();
	void Close();
	bool IsOpen() { return open; }

public:
	ComPort();
	~ComPort();

	void Write(const RobotInput& data);
	RobotOutput Read();
};