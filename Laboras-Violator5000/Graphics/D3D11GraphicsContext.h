#pragma once

#include "Core\NonCopyable.h"

class Window;
class D3D11GraphicsContext : NonCopyable
{
private:
	static D3D11GraphicsContext* s_Instance;

	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendState;

	D3D_FEATURE_LEVEL CreateDevice();
	void CreateSwapChain(const Window& window, IDXGIFactory* dxgiFactory, const DXGI_MODE_DESC& displayMode);
	void CreateBackBufferResources(int width, int height);
	void CreateRasterizerAndBlendStates(int width, int height);

	static inline const D3D11GraphicsContext& GetInstance() { return *s_Instance; }

public:
	D3D11GraphicsContext(Window& window, int requestedWidth = 0, int requestedHeight = 0, bool fullscreen = false);
	~D3D11GraphicsContext();

	static inline ID3D11Device* GetDevice() { return GetInstance().m_Device.Get(); }
	static inline ID3D11DeviceContext* GetDeviceContext() { return GetInstance().m_DeviceContext.Get(); }

	void Clear(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);
	void Present();

	void SetBackBufferAsRenderTarget();
};

inline ID3D11Device* GetD3D11Device() { return D3D11GraphicsContext::GetDevice(); }
inline ID3D11DeviceContext* GetD3D11DeviceContext() { return D3D11GraphicsContext::GetDeviceContext(); }