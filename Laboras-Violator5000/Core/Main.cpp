#include "PrecompiledHeader.h"
#include "GameLoop.h"

int main()
{
#if DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif

	GameLoop().Run();

	return 0;
}