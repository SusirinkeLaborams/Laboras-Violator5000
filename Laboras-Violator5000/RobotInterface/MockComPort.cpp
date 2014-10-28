#include "PrecompiledHeader.h"
#include "MockComPort.h"
#include "RemoteRobot.h"
#include "Utilities\Utilities.h"

MockComPort::MockComPort(const RemoteRobot& robot):
	m_Robot(robot),
	m_Map(Map::CreateSomeMap())
{
}

void MockComPort::Write(const RobotInput& data)
{
	// Do nothing
}

static float GetDistance(const DirectX::XMFLOAT2& left, const DirectX::XMFLOAT2& right)
{
	auto deltaX = left.x - right.x;
	auto deltaY = left.y - right.y;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

RobotOutput MockComPort::Read()
{
	using namespace DirectX;

	RobotOutput robotOutput;
	XMFLOAT2 collisions[2];
	auto endOfBusyWait = Utilities::GetTime() + 0.0016;

	auto position = m_Robot.GetPosition();
	auto rotation = m_Robot.GetRotation();

	auto worldMatrix = XMMatrixTranslation(position.x, position.y, 0.0f);
	auto beam = XMVectorSet(0.0f, 2000.0f, 0.0f, 0.0f);

	collisions[0] = m_Map.GetCollision(Map::Line(position, XMVector3Transform(beam, XMMatrixRotationZ(rotation + 0.261799f) * worldMatrix))); //15° left
	collisions[1] = m_Map.GetCollision(Map::Line(position, XMVector3Transform(beam, XMMatrixRotationZ(rotation - 0.261799f) * worldMatrix))); //15° right

	robotOutput.Magic = RobotOutput::MagicBytes;

	if (collisions[0].x == collisions[0].x && collisions[0].y == collisions[0].y)
	{
		robotOutput.Sensors[0] = static_cast<uint32_t>(58.0f * GetDistance(position, collisions[0]));
	}
	else
	{
		robotOutput.Sensors[0] = 4294967295;
	}

	if (collisions[1].x == collisions[1].x && collisions[1].y == collisions[1].y)
	{
		robotOutput.Sensors[1] = static_cast<uint32_t>(58.0f * GetDistance(position, collisions[1]));
	}
	else
	{
		robotOutput.Sensors[1] = 4294967295;
	}

	robotOutput.Hash = RobotOutput::CalculateHash(robotOutput);

	while (endOfBusyWait > Utilities::GetTime());

	return robotOutput;
}