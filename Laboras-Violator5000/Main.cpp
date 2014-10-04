#include "PrecompiledHeader.h"
#include "D3D11GraphicsContext.h"
#include "Window.h"

int main()
{
#if DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif

	Window w(0, 0, false);
	D3D11GraphicsContext gfxContext(w);

	return 0;
}