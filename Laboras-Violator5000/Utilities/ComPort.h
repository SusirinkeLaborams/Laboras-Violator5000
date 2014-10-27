#pragma once
#include "PrecompiledHeader.h"
#include "RobotInterface\Interface.h"
class ComPort
{
private:
	HANDLE handle;
	bool open;
	std::string name;
	int bytesToRead;
public:
	void Open();
	void Close();
	bool IsOpen(){ return open; }

	RobotOutput Read();

	void Write(const RobotInput &data);

	ComPort(std::string name);
	~ComPort();
};