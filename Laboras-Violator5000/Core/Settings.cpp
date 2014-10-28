#include "PrecompiledHeader.h"
#include "Settings.h"

const std::wstring Settings::kApplicationName = L"Laboras-Violator5000";

// 0 width/height means take desktop resolution
const int Settings::kWidth = 700;
const int Settings::kHeight = 400;
const bool Settings::kFullscreen = false;
const bool Settings::kWindowMovable = true;

const float Settings::kCentimetersInWindowHeight = 500;
const int Settings::kPathTracingBufferSize = 100000;
const float Settings::kPathTracingDecayTime = 5.0f;

const int Settings::kMultisampling = 4;

const std::wstring Settings::RobotConstants::kComPortName = L"COM4";

// Start time
const float Settings::RobotConstants::EngineStartTime::kForward = 0.4f;
const float Settings::RobotConstants::EngineStartTime::kBack = 0.4f;
const float Settings::RobotConstants::EngineStartTime::kLeft = 0.4f;
const float Settings::RobotConstants::EngineStartTime::kRight = 0.4f;

// Stop time
const float Settings::RobotConstants::EngineStopTime::kForward = 0.4f;
const float Settings::RobotConstants::EngineStopTime::kBack = 0.4f;
const float Settings::RobotConstants::EngineStopTime::kLeft = 0.4f;
const float Settings::RobotConstants::EngineStopTime::kRight = 0.4f;

// Speed
const float Settings::RobotConstants::Speed::kForward = 20.0f;
const float Settings::RobotConstants::Speed::kBack = 20.0f;
const float Settings::RobotConstants::Speed::kLeft = DirectX::XM_PI / 4.0f;
const float Settings::RobotConstants::Speed::kRight = DirectX::XM_PI / 4.0f;

const float Settings::RobotConstants::kReadCooldown = 0.2f;
const float Settings::RobotConstants::kWriteCooldown = 0.032f;