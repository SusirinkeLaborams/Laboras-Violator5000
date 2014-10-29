#pragma once

#include <d3d11_2.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>

#include "..\Violator500\RobotIO.h"

#if _DEBUG
#define Assert(x) do { if (!(x) && IsDebuggerPresent()) { __debugbreak(); } } while(false)
#else
#define Assert(x)
#endif

#define USE_MOCK_ROBOT 1