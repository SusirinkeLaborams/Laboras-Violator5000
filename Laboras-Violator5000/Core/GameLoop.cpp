#include "PrecompiledHeader.h"
#include "GameLoop.h"
#include "Input.h"
#include "Settings.h"

GameLoop::GameLoop() :
	m_GraphicsContext(m_Window, Settings::kWidth, Settings::kHeight, Settings::kFullscreen),
	m_Input(Input::GetInstance()),
	m_Camera(static_cast<float>(m_Window.GetWidth()) / static_cast<float>(m_Window.GetHeight()))
{
	OutputDebugStringW((std::to_wstring(reinterpret_cast<uint64_t>(this)) + L"\r\n").c_str());
	m_Receiver.StartReceiving([this]()
	{
		OutputDebugStringW((std::to_wstring(reinterpret_cast<uint64_t>(this)) + L"\r\n").c_str());
	});
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

	const auto& viewMatrix = m_Camera.GetViewProjectionMatrix();
	m_Tank.Render(viewMatrix);

	m_GraphicsContext.Present();
}