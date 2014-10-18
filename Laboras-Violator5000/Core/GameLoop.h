#pragma once

#include "Graphics\D3D11GraphicsContext.h"
#include "Graphics\Environment.h"
#include "Graphics\OrthographicCamera.h"
#include "Graphics\Tank.h"
#include "NonCopyable.h"
#include "RobotInterface\MockRobot.h"
#include "Utilities\CriticalSection.h"
#include "Window.h"

class Input;
class GameLoop : NonCopyable
{
private:
	Window m_Window;
	D3D11GraphicsContext m_GraphicsContext;
	Input& m_Input;
	MockRobot m_Robot;

	OrthographicCamera m_Camera;
	Environment m_Enviroment;
	Tank m_Tank;

	CriticalSection m_CriticalSection;
	DirectX::XMFLOAT3 m_RobotPosition;
	DirectX::XMFLOAT2 m_RobotDirection;
	std::vector<DirectX::XMFLOAT2> m_SensorData;

	void UpdateAndDraw();
	void Update();
	void UpdateRobotData();
	void Render();
	void ReceiveRobotData(const IncomingData& data);

public:
	GameLoop();
	~GameLoop();

	void Run();
};

