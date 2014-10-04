#include "PrecompiledHeader.h"

class ReceiverImpl
{
	std::thread loopThread;
	volatile bool running;
public:
	template<typename Callback>
	void StartReceiving(const Callback &callback);
	void StopReceiving();

private:
	template<typename Callback>
	void StartInternal(const Callback &callback);
};

//not thread safe
template<typename Callback>
void ReceiverImpl::StartReceiving(const Callback &callback)
{
	//joinable == running
	Assert(!(loopThread.joinable() || running));
	running = true;

	loopThread = std::thread(std::bind(&ReceiverImpl::StartInternal, this, std::ref(callback)));
}

template<typename Callback>
void ReceiverImpl::StartInternal(const Callback &callback)
{
	while (running)
	{
		//stuff
	}
}

void ReceiverImpl::StopReceiving()
{
	running = false;
}

using Receiver = ReceiverImpl;