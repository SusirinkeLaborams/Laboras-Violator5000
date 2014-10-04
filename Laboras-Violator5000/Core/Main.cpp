#include "PrecompiledHeader.h"
#include "Graphics\D3D11GraphicsContext.h"
#include "Window.h"

int main()
{
#if DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif

	Window w;
	D3D11GraphicsContext gfxContext(w, 0, 0, true);

	return 0;
}