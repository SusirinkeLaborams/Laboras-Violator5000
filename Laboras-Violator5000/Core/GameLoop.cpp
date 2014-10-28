#include "PrecompiledHeader.h"
#include "GameLoop.h"
#include "Input.h"
#include "Settings.h"
#include "Utilities\Utilities.h"

GameLoop::GameLoop() :
	m_GraphicsContext(m_Window, Settings::kWidth, Settings::kHeight, Settings::kFullscreen),
	m_Input(Input::GetInstance()),
	m_Camera(static_cast<float>(m_Window.GetWidth()) / static_cast<float>(m_Window.GetHeight())),
	m_RobotPosition(0.0f, 0.0f, 0.0f),
	m_RobotRotation(0.0f),
	m_Robot(m_RobotPosition, m_RobotRotation)
{
	m_Robot.GetReceiver().StartReceiving([this](const IncomingData& data)
	{
		ReceiveRobotData(data);
	});
}

GameLoop::~GameLoop()
{
	m_Robot.GetReceiver().StopReceiving();
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

	DirectX::XMFLOAT2 movementVector(0.0f, 0.0f);

	if (m_Input.IsKeyDown(VK_UP))
	{
		movementVector.y += 1.0f;
	}
	if (m_Input.IsKeyDown(VK_DOWN))
	{
		movementVector.y -= 1.0f;
	}
	if (m_Input.IsKeyDown(VK_LEFT))
	{
		movementVector.x -= 1.0f;
	}
	if (m_Input.IsKeyDown(VK_RIGHT))
	{
		movementVector.x += 1.0f;
	}

	// Normalize movement vector

	auto length = sqrt(Utilities::Vector2LengthSqr(movementVector));

	if (length > 0.0f)
	{
		movementVector.x /= length;
		movementVector.y /= length;
	}

	UpdateRobotData();
	m_Robot.SetDirection(movementVector);
}

void GameLoop::UpdateRobotData()
{
	CriticalSection::Lock lock(m_CriticalSection);

	m_Tank.SetPosition(m_RobotPosition);
	m_Camera.SetPosition(DirectX::XMFLOAT3(m_RobotPosition.x, m_RobotPosition.y, m_RobotPosition.z + 1.0f));

	DirectX::XMFLOAT3 rotation(0.0f, 0.0f, m_RobotRotation);
	m_Tank.SetRotation(rotation);
	m_Camera.SetRotation(rotation);

	m_Enviroment.AddData(m_SensorData);
	m_SensorData.clear();
}

void GameLoop::Render()
{
	m_GraphicsContext.SetBackBufferAsRenderTarget();
	m_GraphicsContext.Clear();

	const auto& viewMatrix = m_Camera.GetViewProjectionMatrix();
	m_Enviroment.Render(viewMatrix);
	m_Tank.Render(viewMatrix);

	m_GraphicsContext.Present();
}

void GameLoop::ReceiveRobotData(const IncomingData& data)
{
	CriticalSection::Lock lock(m_CriticalSection);

	m_RobotPosition.x = data.robotPosition.x;
	m_RobotPosition.y = data.robotPosition.y;
	m_RobotRotation = data.robotRotation;

	for (const auto& sensorData : data.data)
	{
		m_SensorData.push_back(sensorData);
	}
}