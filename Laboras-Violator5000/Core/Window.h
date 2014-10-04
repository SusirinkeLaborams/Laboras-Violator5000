#pragma once

class D3D11GraphicsContext;

class Window
{
private:
	HWND m_WindowHandle;
	HMODULE m_ProgramInstance;

	int m_Width;
	int m_Height;
	bool m_Fullscreen;

	void CreateWin32Window();
	void DestroyWin32Window();

	void RegisterForRawInput();
	LRESULT HandleRawInput(WPARAM wParam, LPARAM lParam) const;

	void Initialize(int width, int height, bool fullscreen);

	friend class D3D11GraphicsContext;

public:
	Window();
	~Window();

	void DispatchMessages() const;
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;

	inline bool IsCreated() const { return m_WindowHandle != nullptr; }
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline float GetAspectRatio() const { return static_cast<float>(m_Width) / static_cast<float>(m_Height); }
	inline bool IsFullscreen() const { return m_Fullscreen; }
	inline HWND GetWindowHandle() const { return m_WindowHandle; }
};