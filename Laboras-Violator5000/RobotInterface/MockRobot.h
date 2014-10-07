#pragma once
#include "RobotBase.h"
#include "ReceiverImpl.h"
#include "IncomingData.h"

class MockRobot : public RobotBase<MockRobot>
{
private:
	using Mutex = std::recursive_mutex;
	using Lock = std::lock_guard<Mutex>;

	enum Action
	{
		NONE,
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

private:
	static const float velocity;
	static const float angular;

	Mutex mutex;
	DX::XMFLOAT2 position;
	DX::XMFLOAT2 direction;
	Action action;
	long time;

	//methods
protected:
	void Update();

public:
	MockRobot();
	IncomingData GetData();
};