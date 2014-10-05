#pragma once
#include "RobotBase.h"
#include "ReceiverImpl.h"

class MockRobot : public RobotBase<MockRobot>
{
public:
	MockRobot() :RobotBase(*this){}
};