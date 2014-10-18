#pragma once
#include "RobotBase.h"
#include "ReceiverImpl.h"
#include "IncomingData.h"
#include "Map.h"
#include "Utilities\CriticalSection.h"

class MockRobot : public RobotBase<MockRobot>
{
private:
	using Mutex = CriticalSection;
	using Lock = CriticalSection::Lock;

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
	DirectX::XMFLOAT2 position;
	float rotation;
	Action action;
	float time;
	Map map;

	//methods
protected:
	void Update();
	void Update(Action action);

public:
	MockRobot(DirectX::XMFLOAT3 position, float rotation, Map &&map);
	IncomingData GetData();
	void SetDirection(DirectX::XMFLOAT2 dir);
};