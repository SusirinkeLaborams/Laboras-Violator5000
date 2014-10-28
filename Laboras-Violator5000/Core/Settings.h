#pragma once

namespace Settings
{
	extern const std::wstring kApplicationName;

	extern const int kWidth;
	extern const int kHeight;
	extern const bool kFullscreen;
	extern const bool kWindowMovable;

	extern const float kCentimetersInWindowHeight;
	extern const int kPathTracingBufferSize;
	extern const float kPathTracingDecayTime;

	extern const int kMultisampling;

	namespace RobotConstants
	{
		extern const std::wstring kComPortName;

		namespace EngineStartTime
		{
			extern const float kForward;
			extern const float kBack;
			extern const float kLeft;
			extern const float kRight;
		}

		namespace EngineStopTime
		{
			extern const float kForward;
			extern const float kBack;
			extern const float kLeft;
			extern const float kRight;
		}

		namespace Speed
		{
			extern const float kForward;
			extern const float kBack;
			extern const float kLeft;
			extern const float kRight;
		}

		extern const float kReadCooldown;
		extern const float kWriteCooldown;
	}
}