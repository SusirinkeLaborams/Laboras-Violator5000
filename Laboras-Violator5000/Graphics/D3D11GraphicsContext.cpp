#include "PrecompiledHeader.h"
#include "Core\Settings.h"
#include "Core\Window.h"
#include "D3D11GraphicsContext.h"

using namespace std;

D3D11GraphicsContext* D3D11GraphicsContext::s_Instance;

#include "D3D11GraphicsContextHelpers.inl"

D3D11GraphicsContext::D3D11GraphicsContext(Window& window, int requestedWidth, int requestedHeight, bool fullscreen)
{
	Assert(s_Instance == nullptr);
	s_Instance = this;

	ComPtr<IDXGIFactory1> dxgiFactory;
	ComPtr<IDXGIAdapter1> dxgiAdapter;
	ComPtr<IDXGIOutput> dxgiOutput;

	GetDXGIFactoryAdapterOutput(dxgiFactory, dxgiAdapter, dxgiOutput);
	auto displayMode = GetDisplayMode(dxgiOutput.Get(), requestedWidth, requestedHeight, fullscreen);
	window.Initialize(displayMode.Width, displayMode.Height, fullscreen);

	auto featureLevel = CreateDevice();
	CreateSwapChain(window, dxgiFactory.Get(), displayMode);

	CreateBackBufferResources(window.GetWidth(), window.GetHeight());
	CreateRasterizerAndBlendStates(window.GetWidth(), window.GetHeight());

	if (IsDebuggerPresent())
	{
		PrintAdapterInfo(dxgiAdapter.Get(), featureLevel);
	}
}

D3D11GraphicsContext::~D3D11GraphicsContext()
{
	ComPtr<IDXGIOutput> dxgiOutput;
	BOOL isFullscreen;

	auto result = m_SwapChain->GetFullscreenState(&isFullscreen, &dxgiOutput);

	if (isFullscreen)
	{
		m_SwapChain->SetFullscreenState(FALSE, dxgiOutput.Get());
	}

	Assert(s_Instance != nullptr);
	s_Instance = nullptr;
}

D3D_FEATURE_LEVEL D3D11GraphicsContext::CreateDevice()
{
	UINT deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	D3D_FEATURE_LEVEL supportedFeatureLevel;
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

#if _DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	auto result = CreateDirect3D11Device(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, featureLevels, 
		sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &m_Device, &supportedFeatureLevel, &m_DeviceContext);
	Assert(result == S_OK);

	return supportedFeatureLevel;
}

void D3D11GraphicsContext::CreateSwapChain(const Window& window, IDXGIFactory* dxgiFactory, const DXGI_MODE_DESC& displayMode)
{
	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	GetSwapChainDescription(window.GetWindowHandle(), displayMode, window.IsFullscreen(), swapChainDescription);

	auto result = dxgiFactory->CreateSwapChain(m_Device.Get(), &swapChainDescription, &m_SwapChain);
	Assert(result == S_OK);
}

void D3D11GraphicsContext::CreateBackBufferResources(int width, int height)
{
	ComPtr<ID3D11Texture2D> backBuffer;

	auto result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
	Assert(result == S_OK);

	result = m_Device->CreateRenderTargetView(backBuffer.Get(), NULL, &m_RenderTargetView);
	Assert(result == S_OK);
}

void D3D11GraphicsContext::CreateRasterizerAndBlendStates(int width, int height)
{
	HRESULT result;
	D3D11_RASTERIZER_DESC rasterizerDescription;
	D3D11_BLEND_DESC blendDescription;
	D3D11_VIEWPORT viewport;
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	GetRasterizerStateDescription(rasterizerDescription);
	GetViewPort(width, height, viewport);
	GetBlendStateDescription(blendDescription);

	result = m_Device->CreateRasterizerState(&rasterizerDescription, &m_RasterizerState);
	Assert(result == S_OK);

	result = m_Device->CreateBlendState(&blendDescription, &m_BlendState);
	Assert(result == S_OK);

	m_DeviceContext->RSSetState(m_RasterizerState.Get());
	m_DeviceContext->RSSetViewports(1, &viewport);
	m_DeviceContext->OMSetBlendState(m_BlendState.Get(), blendFactor, 0xFFFFFFFF);
}

void D3D11GraphicsContext::Clear(float red, float green, float blue, float alpha)
{
	float color[] = { red, green, blue, alpha };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
}

void D3D11GraphicsContext::Present()
{
	auto result = m_SwapChain->Present(1, 0);
	Assert(result == S_OK || result == 0x087A0001);
}

void D3D11GraphicsContext::SetBackBufferAsRenderTarget()
{
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}
