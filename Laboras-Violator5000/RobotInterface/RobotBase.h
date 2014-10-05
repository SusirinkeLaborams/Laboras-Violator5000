#pragma once
#include "IncomingData.h"
#include "ReceiverImpl.h"

template<typename Robot>
class RobotBase
{
public:
	using Receiver = ReceiverImpl<Robot>;

private:
	Receiver receiver;

public:
	RobotBase(const Robot &robot);
	Receiver& GetReceiver();
};

template<typename Robot>
typename RobotBase<Robot>::Receiver& RobotBase<Robot>::GetReceiver()
{
	return receiver;
}

template<typename Robot>
RobotBase<Robot>::RobotBase(const Robot &robot)
	:receiver(Receiver(robot))
{
}