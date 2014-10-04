#include "PrecompiledHeader.h"
#include "Input.h"

std::unique_ptr<Input> Input::s_Instance = nullptr;

Input& Input::GetInstance()
{
	if (s_Instance == nullptr)
	{
		s_Instance = std::unique_ptr<Input>(new Input());
	}

	return *s_Instance;
}

Input::Input() :
	m_Quit(false), m_MouseX(0), m_MouseY(0), m_MouseWheelDisplacement(0)
{
	ZeroMemory(m_KeyMap, sizeof(m_KeyMap));
	ZeroMemory(m_MouseButtonMap, sizeof(m_MouseButtonMap));
}

Input::~Input()
{
}

void Input::SetMouseDisplacement(long x, long y)
{
	m_MouseX += x;
	m_MouseY += y;
}

void Input::SetMouseWheelDisplacement(long delta)
{
	m_MouseWheelDisplacement += delta;
}

void Input::HandleMouseDisplacement(long& x, long& y)
{
	x = m_MouseX;
	y = m_MouseY;

	m_MouseX = 0;
	m_MouseY = 0;
}

long Input::HandleMouseWheelDisplacement()
{
	auto returnValue = m_MouseWheelDisplacement;
	m_MouseWheelDisplacement = 0;
	return returnValue;
}

void Input::IgnoreDisplacements()
{
	m_MouseX = 0;
	m_MouseY = 0;
	m_MouseWheelDisplacement = 0;
}