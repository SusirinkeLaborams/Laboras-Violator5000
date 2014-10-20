#include "PrecompiledHeader.h"
#include "MockRobot.h"
#include "Utilities\Utilities.h"

using namespace DirectX;

const float MockRobot::velocity = 30.0f;
const float MockRobot::angular = 1.0f;

MockRobot::MockRobot(DirectX::XMFLOAT3 position, float rotation, Map &&map)
	:RobotBase(*this), 
	position(position.x, position.y), 
	rotation(rotation),
	action(Action::NONE),
	time(static_cast<float>(Utilities::GetTime())),
	map(std::forward<Map>(map))
{

}

IncomingData MockRobot::GetData()
{
	Lock lock(mutex);
	Update();

	IncomingData ret;
	ret.robotPosition = position;
	ret.robotRotation = rotation;

	auto worldMatrix = XMMatrixTranslation(position.x, position.y, 0.0f);
	auto beam = XMVectorSet(0.0f, 2000.0f, 0.0f, 0.0f);

	ret.data[0] = map.GetCollision(Map::Line(position, XMVector3Transform(beam, XMMatrixRotationZ(rotation + 0.261799f) * worldMatrix)));//15 left
	ret.data[1] = map.GetCollision(Map::Line(position, XMVector3Transform(beam, XMMatrixRotationZ(rotation - 0.261799f) * worldMatrix)));//15 right
	ret.data[2] = map.GetCollision(Map::Line(position, XMVector3Transform(beam, XMMatrixRotationZ(rotation + 0.698131f) * worldMatrix)));//40 left
	ret.data[3] = map.GetCollision(Map::Line(position, XMVector3Transform(beam, XMMatrixRotationZ(rotation - 0.698131f) * worldMatrix)));//40 right

	return ret;
}

void MockRobot::Update()
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