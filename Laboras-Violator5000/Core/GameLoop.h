#pragma once

#include "Graphics\D3D11GraphicsContext.h"
#include "Graphics\OrthographicCamera.h"
#include "Graphics\Tank.h"
#include "Window.h"

class Input;
class GameLoop
{
private:
	Window m_Window;
	D3D11GraphicsContext m_GraphicsContext;
	Input& m_Input;

	OrthographicCamera m_Camera;
	Tank m_Tank;

	void UpdateAndDraw();
	void Update();
	void Render();

public:
	GameLoop();
	~GameLoop();

	void Run();
};

