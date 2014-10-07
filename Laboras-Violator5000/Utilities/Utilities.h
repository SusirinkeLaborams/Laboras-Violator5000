#pragma once
#include "PrecompiledHeader.h"

namespace Utilities
{
	inline int64_t GetPerformanceCounterFrequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
	}

	const int64_t s_PerformanceCounterFrequency = GetPerformanceCounterFrequency();

	inline double GetTime()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return static_cast<double>(counter.QuadPart) / s_PerformanceCounterFrequency;
	}

	std::vector<uint8_t> ReadFileToVector(const std::wstring& path);

	template<typename Vector, typename... Vectors>
	inline void _vectorcall Vectors2Transform(DirectX::XMMATRIX matrix, Vector& vector, Vectors&... vectors)
	{
		vector = DirectX::XMVector2Transform(vector, matrix);
		Vectors2Transform(matrix, vectors...);
	}

	template<typename Vector>
	inline void _vectorcall Vectors2Transform(DirectX::XMMATRIX matrix, Vector& vector)
	{
		vector = DirectX::XMVector2Transform(vector, matrix);
	}
}