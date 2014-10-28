#pragma once

class ComPort
{
private:
	HANDLE handle;
	bool open;
	std::string name;

public:
	void Open();
	void Close();
	bool IsOpen(){ return open; }

	RobotOutput Read();

	void Write(const RobotInput &data);

	ComPort(std::string name);
	~ComPort();
};