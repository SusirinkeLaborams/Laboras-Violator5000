typedef HRESULT (WINAPI *D3D11CreateDeviceFunc)(
	_In_opt_ IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	_In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	_Out_opt_ ID3D11Device** ppDevice,
	_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
	_Out_opt_ ID3D11DeviceContext** ppImmediateContext);

typedef HRESULT (WINAPI *CreateDXGIFactory1Func)(REFIID riid, _Out_ void **ppFactory);

#include "Utilities\DllFunctionInvoker.h"

static inline void PrintAdapterInfo(IDXGIAdapter1* dxgiAdapter, D3D_FEATURE_LEVEL featureLevel)
{
	HRESULT result;
	DXGI_ADAPTER_DESC1 adapterDescription;
	wstringstream adapterInfoReport;

	result = dxgiAdapter->GetDesc1(&adapterDescription);
	Assert(result == S_OK);

	adapterInfoReport << "--------------------- Adapter information ---------------------" << endl;
	adapterInfoReport << "\tDescription: \t\t\t" << adapterDescription.Description << endl;
	adapterInfoReport << "\tDedicated memory: \t\t" << adapterDescription.DedicatedVideoMemory / (1024 * 1024) << " MB" << endl;
	adapterInfoReport << "\tShared system memory: \t" << adapterDescription.SharedSystemMemory / (1024 * 1024) << " MB" << endl;
	adapterInfoReport << "\tD3D11 feature level:\t" << (featureLevel >> 12) << "." << ((featureLevel >> 8) & 0xF) << endl;
	adapterInfoReport << "---------------------------------------------------------------" << endl;

	OutputDebugStringW(adapterInfoReport.str().c_str());
}

static inline void GetDXGIFactoryAdapterOutput(ComPtr<IDXGIFactory1>& dxgiFactory, ComPtr<IDXGIAdapter1>& dxgiAdapter, ComPtr<IDXGIOutput>& dxgiOutput)
{
	HRESULT result;

	result = DllFunctionInvoker<CreateDXGIFactory1Func>::Invoke(L"DXGI.dll", "CreateDXGIFactory1", L"This application requires Windows 7/Windows Server 2008 or higher to run.", __uuidof(IDXGIFactory1), &dxgiFactory);	
	Assert(result == S_OK);

	result = dxgiFactory->EnumAdapters1(0, &dxgiAdapter);
	Assert(result == S_OK);

	result = dxgiAdapter->EnumOutputs(0, &dxgiOutput);
	Assert(result == S_OK);
}

template <typename ...Args>
static inline HRESULT CreateDirect3D11Device(Args&&... args)
{
	return DllFunctionInvoker<D3D11CreateDeviceFunc>::Invoke(L"D3D11.dll", "D3D11CreateDevice", L"This application requires DirectX 11 runtime installed to run.", std::forward<Args>(args)...);
}

static inline DXGI_MODE_DESC GetDisplayMode(IDXGIOutput* dxgiOutput, ID3D11Device* device, const Window& window)
{
	DXGI_MODE_DESC desiredMode, displayMode;
	ZeroMemory(&desiredMode, sizeof(desiredMode));

	if (window.IsFullscreen())
	{
		desiredMode.Width = window.GetWidth();
		desiredMode.Height = window.GetHeight();
	}

	desiredMode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;

	auto result = dxgiOutput->FindClosestMatchingMode(&desiredMode, &displayMode, device);
	Assert(result == S_OK);

	// From MSDN:
	// If the buffer width or the buffer height is zero, the sizes will be
	// inferred from the output window size in the swap-chain description.
	displayMode.Width = 0;
	displayMode.Height = 0;

	return displayMode;
}

static inline void GetSwapChainDescription(HWND hWnd, const DXGI_MODE_DESC& displayMode, bool fullscreen, DXGI_SWAP_CHAIN_DESC& swapChainDescription)
{
	ZeroMemory(&swapChainDescription, sizeof(swapChainDescription));

	swapChainDescription.BufferDesc = displayMode;
	swapChainDescription.BufferCount = fullscreen ? 2 : 1;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescription.OutputWindow = hWnd;
	swapChainDescription.Windowed = !fullscreen;

	swapChainDescription.SampleDesc.Count = Settings::kMultisampling;
	swapChainDescription.SampleDesc.Quality = 0;
}

static inline void GetRasterizerStateDescription(D3D11_RASTERIZER_DESC& rasterizerDescription)
{
	ZeroMemory(&rasterizerDescription, sizeof(rasterizerDescription));

	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.FrontCounterClockwise = false;

	rasterizerDescription.DepthBias = 0;
	rasterizerDescription.DepthBiasClamp = 0.0f;
	rasterizerDescription.SlopeScaledDepthBias = 0.0f;

	rasterizerDescription.DepthClipEnable = true;
	rasterizerDescription.ScissorEnable = false;
	rasterizerDescription.MultisampleEnable = true;
	rasterizerDescription.AntialiasedLineEnable = false;
}

static inline void GetViewPort(int width, int height, D3D11_VIEWPORT& viewport)
{
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

static inline void GetBlendStateDescription(D3D11_BLEND_DESC& blendDescription)
{
	ZeroMemory(&blendDescription, sizeof(blendDescription));

	blendDescription.AlphaToCoverageEnable = false;
	blendDescription.IndependentBlendEnable = false;

	blendDescription.RenderTarget[0].BlendEnable = false;
	blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}