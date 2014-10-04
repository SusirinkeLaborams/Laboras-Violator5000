#pragma once

class Input
{
private:
	static std::unique_ptr<Input> s_Instance;

	bool m_Quit;
	bool m_KeyMap[256];
	bool m_MouseButtonMap[6];

	long m_MouseX, m_MouseY;
	long m_MouseWheelDisplacement;

	Input();
	Input(const Input&);

public:
	~Input();
	static Input& GetInstance();

	inline void KeyDown(int key) { m_KeyMap[key] = true; }
	inline void KeyUp(int key) { m_KeyMap[key] = false; }
	inline bool IsKeyDown(int key) const { return m_KeyMap[key]; }

	inline void MouseButtonDown(int buttonNumber) { m_MouseButtonMap[buttonNumber] = true; }
	inline void MouseButtonUp(int buttonNumber) { m_MouseButtonMap[buttonNumber] = false; }
	inline bool IsMouseButtonDown(int buttonNumber) { return m_MouseButtonMap[buttonNumber]; }

	void SetMouseDisplacement(long x, long y);
	void SetMouseWheelDisplacement(long delta);
	void HandleMouseDisplacement(long& x, long& y);
	long HandleMouseWheelDisplacement();

	void IgnoreDisplacements();

	inline void Quit() { m_Quit = true; }
	inline bool ShouldQuit() const { return m_Quit; }
};

