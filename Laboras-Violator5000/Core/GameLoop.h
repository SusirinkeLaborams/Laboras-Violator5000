#pragma once

#include "Graphics\D3D11GraphicsContext.h"
#include "Window.h"

class Input;
class GameLoop
{
private:
	Window m_Window;
	D3D11GraphicsContext m_GraphicsContext;
	Input& m_Input;

	void UpdateAndDraw();
	void Update();
	void Render();

public:
	GameLoop();
	~GameLoop();

	void Run();
};

