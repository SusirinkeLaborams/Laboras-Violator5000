#pragma once

class D3D11GraphicsContext
{
private:
	static D3D11GraphicsContext* s_Instance;

	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	ComPtr<IDXGISwapChain> m_SwapChain;
	ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	ComPtr<ID3D11RasterizerState> m_RasterizerState;
	ComPtr<ID3D11BlendState> m_BlendState;

	D3D_FEATURE_LEVEL CreateDeviceAndSwapChain(HWND hWnd, IDXGIFactory* dxgiFactory, IDXGIOutput* dxgiOutput, int width, int height, bool fullscreen);
	void CreateBackBufferResources(int width, int height);
	void CreateRasterizerAndBlendStates(int width, int height);

	static inline const D3D11GraphicsContext& GetInstance() { return *s_Instance; }

public:
	D3D11GraphicsContext(HWND hWnd, int width, int height, bool fullscreen);
	~D3D11GraphicsContext();

	static inline ID3D11Device* GetDevice() { return GetInstance().m_Device.Get(); }
	static inline ID3D11DeviceContext* GetDeviceContext() { return GetInstance().m_DeviceContext.Get(); }

	void Clear(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);
	void Present();

	void SetBackBufferAsRenderTarget();
};

inline ID3D11Device* GetD3D11Device() { return D3D11GraphicsContext::GetDevice(); }
inline ID3D11DeviceContext* GetD3D11DeviceContext() { return D3D11GraphicsContext::GetDeviceContext(); }
