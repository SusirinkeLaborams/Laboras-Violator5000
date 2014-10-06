#pragma once
#include "RobotBase.h"
#include "ReceiverImpl.h"
#include "IncomingData.h"

class MockRobot : public RobotBase<MockRobot>
{
	DX::XMFLOAT2 position;
public:
	MockRobot();
	IncomingData GetData();
};