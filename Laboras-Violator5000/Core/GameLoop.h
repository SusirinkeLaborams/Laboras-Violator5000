#pragma once

#include "Graphics\D3D11GraphicsContext.h"
#include "Graphics\Environment.h"
#include "Graphics\OrthographicCamera.h"
#include "Graphics\Tank.h"
#include "NonCopyable.h"
#include "RobotInterface\MockRobot.h"
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

	void UpdateAndDraw();
	void Update();
	void Render();
	void ReceiveRobotData(const IncomingData& data);

public:
	GameLoop();
	~GameLoop();

	void Run();
};

