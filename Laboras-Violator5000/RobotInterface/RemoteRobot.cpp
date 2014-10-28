#include "PrecompiledHeader.h"
#include "RemoteRobot.h"
#include "Core\Settings.h"
#include "Utilities\Utilities.h"

using namespace DirectX;

RemoteRobot::RemoteRobot(DirectX::XMFLOAT3 position, float rotation) :
	RobotBase(*this),
	m_Position(position.x, position.y),
	m_Rotation(rotation),
	m_LocalAction(Action::NONE),
	m_RemoteAction(Action::NONE),
	m_LastUpdateTime(static_cast<float>(Utilities::GetTime()))
#if USE_MOCK_ROBOT
	, m_ComPort(*this)
#endif
{
	ZeroMemory(&m_Timers, sizeof(m_Timers));
}

IncomingData RemoteRobot::GetData()
{
	IncomingData ret;
	Update();

	{
		Lock lock(mutex);

		ret.robotPosition = m_Position;
		ret.robotRotation = m_Rotation;
	}

	auto data = m_ComPort.Read();

	float distance[kSensorCount];

	for (int i = 0; i < kSensorCount; i++)
	{
		distance[i] = data.Sensors[i] / 58.0f;
	}

	Assert(kSensorCount == 2);
	ret.data[0] = XMFLOAT2(m_Position.x - sin(m_Rotation + 0.261799f) * distance[0], m_Position.y + cos(m_Rotation + 0.261799f) * distance[0]);
	ret.data[1] = XMFLOAT2(m_Position.x - sin(m_Rotation - 0.261799f) * distance[1], m_Position.y + cos(m_Rotation - 0.261799f) * distance[1]);

	return ret;
}

static float GetStartTime(RemoteRobot::Action action)
{
	switch (action)
	{
	case RemoteRobot::Action::FORWARD:
		return Settings::RobotConstants::EngineStartTime::kForward;

	case RemoteRobot::Action::BACKWARD:
		return Settings::RobotConstants::EngineStartTime::kBack;

	case RemoteRobot::Action::LEFT:
		return Settings::RobotConstants::EngineStartTime::kLeft;

	case RemoteRobot::Action::RIGHT:
		return Settings::RobotConstants::EngineStartTime::kRight;
	}

	return 0.0f;
}

static float GetStopTime(RemoteRobot::Action action)
{
	switch (action)
	{
	case RemoteRobot::Action::FORWARD:
		return Settings::RobotConstants::EngineStopTime::kForward;

	case RemoteRobot::Action::BACKWARD:
		return Settings::RobotConstants::EngineStopTime::kBack;

	case RemoteRobot::Action::LEFT:
		return Settings::RobotConstants::EngineStopTime::kLeft;

	case RemoteRobot::Action::RIGHT:
		return Settings::RobotConstants::EngineStopTime::kRight;
	}

	return 0.0f;
}

void RemoteRobot::Update()
{
	Action myAction;

	{
		Lock lock(mutex);
		UpdateActions();

		if (m_LocalAction != m_RemoteAction)
		{
			auto currentTime = static_cast<float>(Utilities::GetTime());
			m_Timers[m_LocalAction].startTime = currentTime + GetStartTime(m_LocalAction);
			m_Timers[m_LocalAction].endTime = currentTime + 10000.0f;	// Never end
			m_Timers[m_RemoteAction].endTime = currentTime + GetStopTime(m_RemoteAction);

			m_RemoteAction = m_LocalAction;
		}

		myAction = m_LocalAction;
	}

	SendCommand(myAction);
}

void RemoteRobot::SetAction(Action action)
{
	Lock lock(mutex);
	m_LocalAction = action;
}

void RemoteRobot::SendCommand(Action action)
{
	RobotInput command;

	switch (action)
	{
	case RemoteRobot::NONE:
		command.DirectionL = 0;
		command.DirectionR = 0;
		command.PowerL = 0;
		command.PowerR = 0;
		break;
	case RemoteRobot::FORWARD:
		command.DirectionL = 0;
		command.DirectionR = 0;
		command.PowerL = 7;
		command.PowerR = 7;
		break;
	case RemoteRobot::BACKWARD:
		command.DirectionL = 1;
		command.DirectionR = 1;
		command.PowerL = 7;
		command.PowerR = 7;
		break;
	case RemoteRobot::LEFT:
		command.DirectionL = 1;
		command.DirectionR = 0;
		command.PowerL = 7;
		command.PowerR = 7;
		break;
	case RemoteRobot::RIGHT:
		command.DirectionL = 0;
		command.DirectionR = 1;
		command.PowerL = 7;
		command.PowerR = 7;
		break;
	}

	m_ComPort.Write(command);
}

void RemoteRobot::UpdateActions()
{
	auto currentTime = static_cast<float>(Utilities::GetTime());
	auto deltaTime = currentTime - m_LastUpdateTime;

	for (int i = Action::FORWARD; i < Action::ACTION_COUNT; i++)
	{
		if (m_Timers[i].startTime < currentTime && m_Timers[i].endTime > currentTime)
		{
			auto duration = min(m_Timers[i].endTime, currentTime) - max(m_Timers[i].startTime, m_LastUpdateTime);
			SimulateAction(static_cast<Action>(i), duration);
		}
	}

	m_LastUpdateTime = currentTime;
}

void RemoteRobot::SimulateAction(Action action, float duration)
{
	float distance;

	switch (action)
	{
	case Action::FORWARD:
		distance = Settings::RobotConstants::Speed::kForward * duration;
		m_Position.x += -sin(m_Rotation) * distance;
		m_Position.y += cos(m_Rotation) * distance;
		break;

	case Action::BACKWARD:
		distance = Settings::RobotConstants::Speed::kForward * duration;
		m_Position.x -= -sin(m_Rotation) * distance;
		m_Position.y -= cos(m_Rotation) * distance;
		break;

	case Action::LEFT:
		m_Rotation += Settings::RobotConstants::Speed::kLeft * duration;
		break;

	case Action::RIGHT:
		m_Rotation -= Settings::RobotConstants::Speed::kRight * duration;
		break;
	}
}