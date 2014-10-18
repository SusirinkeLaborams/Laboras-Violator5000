#pragma once
#include "PrecompiledHeader.h"
#include "Core\NonCopyable.h"

template<typename Robot>
class ReceiverImpl : NonCopyable
{
	std::thread loopThread;
	volatile bool running;
	Robot &robot;
public:
	ReceiverImpl(Robot &robot);
	ReceiverImpl(ReceiverImpl&&) = delete;

	~ReceiverImpl();

	template<typename Callback>
	void StartReceiving(Callback callback);
	void StopReceiving();

private:
	template<typename Callback>
	void StartInternal(Callback callback);
};

template<typename Robot>
ReceiverImpl<Robot>::ReceiverImpl(Robot &robot)
	:robot(robot), running(false)
{
}

template<typename Robot>
ReceiverImpl<Robot>::~ReceiverImpl()
{
	StopReceiving();
}

//not thread safe
template<typename Robot>
template<typename Callback>
void ReceiverImpl<Robot>::StartReceiving(Callback callback)
{
	//joinable == running
	Assert(!(loopThread.joinable() || running));
	running = true;

	loopThread = std::thread(std::bind(&ReceiverImpl<Robot>::StartInternal<Callback>, this, callback));
}

template<typename Robot>
template<typename Callback>
void ReceiverImpl<Robot>::StartInternal(Callback callback)
{
	while (running)
	{
		for (int i = 0; i < 10; i++)
		{
			callback(robot.GetData());
		}

		Sleep(16);
	}
}

template<typename Robot>
void ReceiverImpl<Robot>::StopReceiving()
{
	running = false;
	if (loopThread.joinable())
		loopThread.join();
}