#include "PrecompiledHeader.h"
#include "RemoteRobot.h"
#include "Utilities\Utilities.h"

using namespace DirectX;

const float RemoteRobot::velocity = 30.0f;
const float RemoteRobot::angular = 1.0f;

RemoteRobot::RemoteRobot(DirectX::XMFLOAT3 position, float rotation)
	:RobotBase(*this),
	position(position.x, position.y),
	rotation(rotation),
	action(Action::NONE),
	time(static_cast<float>(Utilities::GetTime())),
	port("COM1")
{

}

IncomingData RemoteRobot::GetData()
{
	auto till = Utilities::GetTime() + 0.0016;
	IncomingData ret;

	{
		Lock lock(mutex);
		Update();

		ret.robotPosition = position;
		ret.robotRotation = rotation;

		
		if (port.IsOpen())
		{
			auto data = port.Read();
			ret.data[0] = XMFLOAT2(cos(0.261799f) * data.Sensors[0], sin(0.261799f) * data.Sensors[0]);
			ret.data[1] = XMFLOAT2(cos(-0.261799f) * data.Sensors[1], sin(-0.261799f) * data.Sensors[1]);
		}
	}

	while (till > Utilities::GetTime());

	return ret;
}

void RemoteRobot::Update()
{
	Lock lock(mutex);

	auto now = static_cast<float>(Utilities::GetTime());
	auto passed = now - time;

	if (action == Action::FORWARD)
	{
		auto matrix = XMMatrixRotationZ(rotation);
		auto dir = XMVector3Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), matrix);
		auto pos = XMLoadFloat2(&position) + dir * (velocity * passed);
		XMStoreFloat2(&position, pos);
	}
	else if (action == Action::BACKWARD)
	{
		auto matrix = XMMatrixRotationZ(rotation);
		auto dir = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		auto pos = XMLoadFloat2(&position) - XMVector3Transform(dir, matrix) * (velocity * passed);
		XMStoreFloat2(&position, pos);
	}
	else if (action == Action::LEFT)
	{
		rotation += angular * passed;
	}
	else if (action == Action::RIGHT)
	{
		rotation -= angular * passed;
	}

	time = now;
}

void RemoteRobot::Update(Action newAction)
{
	Lock lock(mutex);
	if (port.IsOpen())
		port.Write(InputFromAction(newAction));
	Update();
	action = newAction;
}

void RemoteRobot::SetDirection(DirectX::XMFLOAT2 dir)
{
	Action newAction;

	//tingiu skaiciuot
	if (Utilities::Vector2LengthSqr(dir) <= 0.01f)
	{
		newAction = Action::NONE;
	}
	else
	{
		if (abs(dir.y) > abs(dir.x))
		{
			if (dir.y > 0.0f)
				newAction = Action::FORWARD;
			else
				newAction = Action::BACKWARD;
		}
		else
		{
			if (dir.x > 0.0f)
				newAction = Action::RIGHT;
			else
				newAction = Action::LEFT;
		}
	}
	Update(newAction);
}

RobotInput RemoteRobot::InputFromAction(Action action)
{
	RobotInput ret;
	switch (action)
	{
	case RemoteRobot::NONE:
		ret.DirectionL = 0;
		ret.DirectionR = 0;
		ret.PowerL = 0;
		ret.PowerR = 0;
		break;
	case RemoteRobot::FORWARD:
		ret.DirectionL = 0;
		ret.DirectionR = 0;
		ret.PowerL = 7;
		ret.PowerR = 7;
		break;
	case RemoteRobot::BACKWARD:
		ret.DirectionL = 1;
		ret.DirectionR = 1;
		ret.PowerL = 7;
		ret.PowerR = 7;
		break;
	case RemoteRobot::LEFT:
		ret.DirectionL = 1;
		ret.DirectionR = 0;
		ret.PowerL = 7;
		ret.PowerR = 7;
		break;
	case RemoteRobot::RIGHT:
		ret.DirectionL = 0;
		ret.DirectionR = 1;
		ret.PowerL = 7;
		ret.PowerR = 7;
		break;
	}
	return ret;
}