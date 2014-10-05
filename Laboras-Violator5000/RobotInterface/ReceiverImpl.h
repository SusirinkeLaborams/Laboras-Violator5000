#pragma once
#include "PrecompiledHeader.h"

template<typename Robot>
class ReceiverImpl
{
	std::thread loopThread;
	volatile bool running;
	const Robot &robot;
public:
	ReceiverImpl(const Robot &robot);
	ReceiverImpl(const ReceiverImpl&) = delete;
	ReceiverImpl(ReceiverImpl&&) = delete;

	template<typename Callback>
	void StartReceiving(Callback callback);
	void StopReceiving();

private:
	template<typename Callback>
	void StartInternal(Callback callback);
};

template<typename Robot>
ReceiverImpl<Robot>::ReceiverImpl(const Robot &robot)
	:robot(robot)
{
}

//not thread safe
template<typename Robot>
template<typename Callback>
void ReceiverImpl<Robot>::StartReceiving(Callback callback)
{
	//joinable == running
	Assert(!(loopThread.joinable() || running));
	running = true;

	loopThread = std::thread(std::bind(&ReceiverImpl::StartInternal, this, callback));
}

template<typename Robot>
template<typename Callback>
void ReceiverImpl<Robot>::StartInternal(Callback callback)
{
	while (running)
	{
		//stuff
	}
}

template<typename Robot>
void ReceiverImpl<Robot>::StopReceiving()
{
	running = false;
}