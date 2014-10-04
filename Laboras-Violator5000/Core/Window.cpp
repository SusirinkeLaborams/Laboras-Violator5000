#include "PrecompiledHeader.h"
#include "Input.h"
#include "Settings.h"
#include "Window.h"

Window::Window() :
	m_WindowHandle(nullptr),
	m_ProgramInstance(GetModuleHandle(nullptr)), 
	m_Width(0),
	m_Height(0),
	m_Fullscreen(false)
{
}

Window::~Window()
{
	DestroyWin32Window();
}

void Window::Initialize(int width, int height, bool fullscreen)
{
	m_Width = width;
	m_Height = height;
	m_Fullscreen = fullscreen;

	CreateWin32Window();
	RegisterForRawInput();
}

void Window::CreateWin32Window()
{
	WNDCLASSEX windowInfo;
	DEVMODE screenSettings;
	int posX, posY;

	ZeroMemory(&windowInfo, sizeof(WNDCLASSEX));
	windowInfo.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowInfo.hInstance = m_ProgramInstance;
	windowInfo.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowInfo.hIconSm = windowInfo.hIcon;
	windowInfo.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowInfo.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	windowInfo.lpszMenuName = NULL;
	windowInfo.lpszClassName = Settings::kApplicationName.c_str();
	windowInfo.cbSize = sizeof(WNDCLASSEX);

	windowInfo.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
	{
		return reinterpret_cast<Window*>(lParam)->HandleMessage(hWnd, uMsg, wParam, lParam);
	};

	RegisterClassEx(&windowInfo);

	if (m_Fullscreen)
	{
		ZeroMemory(&screenSettings, sizeof(screenSettings));

		screenSettings.dmSize = sizeof(DEVMODE);
		screenSettings.dmPelsWidth = static_cast<DWORD>(m_Width);
		screenSettings.dmPelsHeight = static_cast<DWORD>(m_Height);
		screenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_Width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_Height) / 2;
	}

	m_WindowHandle = CreateWindowEx(WS_EX_APPWINDOW, windowInfo.lpszClassName, windowInfo.lpszClassName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, m_Width, m_Height, nullptr, nullptr, m_ProgramInstance, this);
	Assert(m_WindowHandle != nullptr);

	ShowWindow(m_WindowHandle, SW_SHOW);
	SetForegroundWindow(m_WindowHandle);
	SetFocus(m_WindowHandle);
	ShowCursor(FALSE);
}

void Window::DestroyWin32Window()
{
	if (m_Fullscreen)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	DestroyWindow(m_WindowHandle);
	m_WindowHandle = nullptr;

	UnregisterClass(Settings::kApplicationName.c_str(), m_ProgramInstance);
}

void Window::RegisterForRawInput()
{
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;					// Mouse
	Rid[0].dwFlags = 0;
	Rid[0].hwndTarget = m_WindowHandle;

	if (m_Fullscreen)
	{
		Rid[0].dwFlags &= RIDEV_CAPTUREMOUSE;
	}

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;					// Keyboard
	Rid[1].dwFlags = 0;
	Rid[1].hwndTarget = m_WindowHandle;

	auto result = RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]));
	Assert(result != 0);
}

void Window::DispatchMessages() const
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT Window::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	switch (uMsg)
	{
	case WM_DESTROY:
	case WM_QUIT:
		Input::GetInstance().Quit();
		return 0;

	case WM_INPUT:
		return HandleRawInput(wParam, lParam);

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT Window::HandleRawInput(WPARAM wParam, LPARAM lParam) const
{
	unsigned int dataSize = 0;
	auto& input = Input::GetInstance();

	auto result = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
	auto buffer = std::unique_ptr<unsigned char[]>(new unsigned char[dataSize]);
	result = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer.get(), &dataSize, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = (RAWINPUT*)buffer.get();

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		if (raw->data.keyboard.Message == WM_KEYDOWN && wParam == 0)
		{
			input.KeyDown(raw->data.keyboard.VKey);
		}
		else if (raw->data.keyboard.Message == WM_KEYUP)
		{
			input.KeyUp(raw->data.keyboard.VKey);
		}
		return 0;
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE && wParam == 0)
	{
		switch (raw->data.mouse.usButtonFlags)
		{
		case RI_MOUSE_BUTTON_1_DOWN:
			input.MouseButtonDown(1);
			break;

		case RI_MOUSE_BUTTON_1_UP:
			input.MouseButtonUp(1);
			break;

		case RI_MOUSE_BUTTON_2_DOWN:
			input.MouseButtonDown(2);
			break;

		case RI_MOUSE_BUTTON_2_UP:
			input.MouseButtonUp(2);
			break;

		case RI_MOUSE_BUTTON_3_DOWN:
			input.MouseButtonDown(3);
			break;

		case RI_MOUSE_BUTTON_3_UP:
			input.MouseButtonUp(3);
			break;

		case RI_MOUSE_BUTTON_4_DOWN:
			input.MouseButtonDown(4);
			break;

		case RI_MOUSE_BUTTON_4_UP:
			input.MouseButtonUp(4);
			break;

		case RI_MOUSE_BUTTON_5_DOWN:
			input.MouseButtonDown(5);
			break;

		case RI_MOUSE_BUTTON_5_UP:
			input.MouseButtonUp(5);
			break;

		case RI_MOUSE_WHEEL:
			input.SetMouseWheelDisplacement(static_cast<long>(raw->data.mouse.usButtonData));
			break;
		}

		input.SetMouseDisplacement(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
		return 0;
	}

	return DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER));
}