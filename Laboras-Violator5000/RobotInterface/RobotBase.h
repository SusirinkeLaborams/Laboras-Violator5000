#pragma once

#include "ReceiverImpl.h"
#include "Utilities\CriticalSection.h"

using int16 = short;
using uint8 = byte;
using int8 = char;

template<typename Robot>
class RobotBase : NonCopyable
{
	//types
protected:
	using Mutex = CriticalSection;
	using Lock = CriticalSection::Lock;
public:
	using Receiver = ReceiverImpl<Robot>;

	//fields
private:
	Receiver receiver;
protected:
	Mutex mutex;

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