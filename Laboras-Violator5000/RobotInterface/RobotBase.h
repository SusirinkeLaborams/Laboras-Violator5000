#pragma once
#include "IncomingData.h"
#include "ReceiverImpl.h"

using int16 = short;
using uint8 = byte;
using int8 = char;

template<typename Robot>
class RobotBase : NonCopyable
{
	//types
public:
	using Receiver = ReceiverImpl<Robot>;

	//fields
private:
	Receiver receiver;

public:
	RobotBase(Robot &robot);
	Receiver& GetReceiver();
};

template<typename Robot>
typename RobotBase<Robot>::Receiver& RobotBase<Robot>::GetReceiver()
{
	return receiver;
}

template<typename Robot>
RobotBase<Robot>::RobotBase(Robot &robot)
	:receiver(robot)
{
}