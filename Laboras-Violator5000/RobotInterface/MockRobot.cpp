#include "PrecompiledHeader.h"
#include "MockRobot.h"
#include "Utilities\Utilities.h"

using namespace DirectX;

const float MockRobot::velocity = 1.0f;
const float MockRobot::angular = 1.0f;

MockRobot::MockRobot(Map &&map)
	:RobotBase(*this), 
	position(0.0f, 0.0f), 
	direction(0.0f, 1.0f),
	action(Action::NONE),
	time(Utilities::GetTime()),
	map(std::forward<Map>(map))
{

}

IncomingData MockRobot::GetData()
{
	Update();

	IncomingData ret;
	ret.robotPosition = position;

	ret.data[0] = map.GetCollision(Map::Line(position, XMFLOAT2(position.x - 300.0f, position.y + 200.0f)));
	ret.data[1] = map.GetCollision(Map::Line(position, XMFLOAT2(position.x - 100.0f, position.y + 300.0f)));
	ret.data[2] = map.GetCollision(Map::Line(position, XMFLOAT2(position.x + 100.0f, position.y + 300.0f)));
	ret.data[3] = map.GetCollision(Map::Line(position, XMFLOAT2(position.x + 300.0f, position.y + 200.0f)));

	return ret;
}

void MockRobot::Update()
{
	Lock lock(mutex);

	auto now = Utilities::GetTime();
	auto passed = now - time;

	if (action == Action::FORWARD)
	{
		auto pos = XMLoadFloat2(&position) + XMLoadFloat2(&direction) * (velocity * passed);
		XMStoreFloat2(&position, pos);
	}
	else if (action == Action::BACKWARD)
	{
		auto pos = XMLoadFloat2(&position) - XMLoadFloat2(&direction) * (velocity * passed);
		XMStoreFloat2(&position, pos);
	}
	else if (action == Action::LEFT)
	{
		auto dir = XMLoadFloat2(&direction);
		auto matrix = XMMatrixRotationZ(angular * passed);
		dir = XMVector3Transform(dir, matrix);
		XMStoreFloat2(&direction, dir);
	}
	else if (action == Action::RIGHT)
	{
		auto dir = XMLoadFloat2(&direction);
		auto matrix = XMMatrixRotationZ(-angular * passed);
		dir = XMVector3Transform(dir, matrix);
		XMStoreFloat2(&direction, dir);
	}

	time = now;
}

void MockRobot::Update(Action newAction)
{
	Lock lock(mutex);
	Update();
	action = newAction;
}

void MockRobot::SetDirection(DirectX::XMFLOAT2 dir)
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