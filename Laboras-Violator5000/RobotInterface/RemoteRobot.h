#pragma once
#include "RobotBase.h"
#include "Utilities\ComPort.h"

class RemoteRobot : RobotBase < RemoteRobot >
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
	ComPort port;

	//methods
	static RobotInput InputFromAction(Action action);
protected:
	void Update();
	void Update(Action action);

public:
	RemoteRobot(DirectX::XMFLOAT3 position, float rotation);
	IncomingData GetData();
	void SetDirection(DirectX::XMFLOAT2 dir);
};