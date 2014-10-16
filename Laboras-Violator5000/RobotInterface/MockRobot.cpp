#include "PrecompiledHeader.h"
#include "MockRobot.h"

using namespace DX;

const float MockRobot::velocity = 1.0f;
const float MockRobot::angular = 1.0f;

MockRobot::MockRobot()
	:RobotBase(*this), 
	position(0.0f, 0.0f), 
	direction(0.0f, 1.0f),
	action(Action::NONE),
	time(clock())
{

}

IncomingData MockRobot::GetData()
{
	Update();

	IncomingData ret;
	ret.robotPosition = position;
	ret.data[0] = DX::XMFLOAT2(-2.0f, 2.0f);
	ret.data[0] = DX::XMFLOAT2(-1.0f, 3.0f);
	ret.data[0] = DX::XMFLOAT2(2.0f, 2.0f);
	ret.data[0] = DX::XMFLOAT2(1.0f, 3.0f);

	return ret;
}


void MockRobot::Update()
{
	Lock lock(mutex);

	long now = clock();
	long passed = now - time;

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


void MockRobot::SetDirection(DX::XMFLOAT2 dir)
{

}