#pragma once

template <typename F>
struct return_type;

template <typename Ret, typename... Args>
struct return_type<Ret(*)(Args...)>
{
	typedef Ret value;
};

template <typename Ret, typename... Args>
struct return_type<Ret(__stdcall*)(Args...)>
{
	typedef Ret value;
};

template <typename FuncPtr>
class DllFunctionInvoker
{
public:
	DllFunctionInvoker() = delete;
	DllFunctionInvoker(const DllFunctionInvoker&) = delete;
	~DllFunctionInvoker() = delete;

	template <typename ...Args>
	static inline typename return_type<FuncPtr>::value Invoke(const wchar_t* dllName, const char* functionName, const wchar_t* errorMessageOnFailure, Args&&... args)
	{
		auto module = LoadLibraryExW(dllName, nullptr, 0);

		if (module != nullptr)
		{
			auto funcAddress = GetProcAddress(module, functionName);

			if (funcAddress != nullptr)
			{
				return reinterpret_cast<FuncPtr>(funcAddress)(std::forward<Args>(args)...);
			}
		}

		Assert(false);
		MessageBoxW(nullptr, L"Fatal error", errorMessageOnFailure, MB_OK | MB_ICONERROR);
		exit(-1);
	}
};
