#pragma once

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
}