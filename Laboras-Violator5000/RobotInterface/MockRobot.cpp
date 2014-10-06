#include "PrecompiledHeader.h"
#include "MockRobot.h"

MockRobot::MockRobot()
	:RobotBase(*this), position(0.0f, 0.0f)
{

}

IncomingData MockRobot::GetData()
{
	IncomingData ret;
	ret.robotPosition = position;
	ret.data[0] = DX::XMFLOAT2(-2.0f, 2.0f);
	ret.data[0] = DX::XMFLOAT2(-1.0f, 3.0f);
	ret.data[0] = DX::XMFLOAT2(2.0f, 2.0f);
	ret.data[0] = DX::XMFLOAT2(1.0f, 3.0f);

	return ret;
}