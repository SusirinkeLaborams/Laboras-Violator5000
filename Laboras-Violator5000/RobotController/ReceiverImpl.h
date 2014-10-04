#include "PrecompiledHeader.h"

class ReceiverImpl
{
public:
	template<typename Callback>
	void StartReceiving(Callback &&callback){}
	void StopReceiving(){}
};

using Receiver = ReceiverImpl;