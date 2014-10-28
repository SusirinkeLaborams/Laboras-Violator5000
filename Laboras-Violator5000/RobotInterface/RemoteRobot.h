#pragma once

#include "RobotBase.h"

#if !USE_MOCK_ROBOT
#include "Utilities\ComPort.h"
#else
#include "MockComPort.h"
using ComPort = MockComPort;
#endif

class RemoteRobot : public RobotBase<RemoteRobot>
{
public:
	enum Action
	{
		NONE,
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		ACTION_COUNT
	};

private:
	struct ActionTimer
	{
		float startTime;
		float endTime;
	};

	ActionTimer m_Timers[Action::ACTION_COUNT];
	Action m_LocalAction;
	Action m_RemoteAction;
	float m_LastUpdateTime;

	DirectX::XMFLOAT2 m_Position;
	float m_Rotation;

	ComPort m_ComPort;

	void SendCommand(Action action);
	void SimulateAction(Action action, float duration);
	void Update();
	void UpdateActions();

public:
	RemoteRobot(DirectX::XMFLOAT3 position, float rotation);

	IncomingData GetData();
	void SetAction(Action action);

	DirectX::XMFLOAT2 GetPosition() const { return m_Position; };
	float GetRotation() const { return m_Rotation; };
};