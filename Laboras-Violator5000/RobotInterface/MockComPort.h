#pragma once
#include "RobotBase.h"
#include "ReceiverImpl.h"
#include "Map.h"

class RemoteRobot;

class MockComPort
{
private:
	const RemoteRobot& m_Robot;
	Map m_Map;

public:
	MockComPort(const RemoteRobot& robot);

	void Write(const RobotInput& data);
	RobotOutput Read();
};