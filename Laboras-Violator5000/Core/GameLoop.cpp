#include "PrecompiledHeader.h"
#include "GameLoop.h"
#include "Input.h"
#include "Settings.h"

GameLoop::GameLoop() :
	m_GraphicsContext(m_Window, Settings::kWidth, Settings::kHeight, Settings::kFullscreen),
	m_Input(Input::GetInstance())
{
}

GameLoop::~GameLoop()
{
}

void GameLoop::Run()
{
	while (!m_Input.ShouldQuit())
	{
		m_Window.DispatchMessages();
		UpdateAndDraw();

#if _DEBUG
		Assert(_CrtCheckMemory() == TRUE);
#endif
	}
}

void GameLoop::UpdateAndDraw()
{
	Update();
	Render();
}

void GameLoop::Update()
{
	if (m_Input.IsKeyDown(VK_ESCAPE))
	{
		m_Input.Quit();
	}
}

void GameLoop::Render()
{
	m_GraphicsContext.SetBackBufferAsRenderTarget();
	m_GraphicsContext.Clear();
	m_GraphicsContext.Present();
}