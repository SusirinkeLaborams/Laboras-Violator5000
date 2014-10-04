#pragma once

class NonCopyable
{
public:
	inline NonCopyable() {}
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
};