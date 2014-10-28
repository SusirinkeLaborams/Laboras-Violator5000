#pragma once
#include "RobotBase.h"
#include "ReceiverImpl.h"
#include "Map.h"

class MockRobot : public RobotBase<MockRobot>
{
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
	MockRobot(DirectX::XMFLOAT3 position, float rotation);
	IncomingData GetData();
	void SetDirection(DirectX::XMFLOAT2 dir);
};