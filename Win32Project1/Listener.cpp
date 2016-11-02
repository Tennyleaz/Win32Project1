#include "stdafx.h"
#include "Listener.h"

LRESULT Listener::Trig(UINT msg, Parameter& param)
{
	if (evnetHandlerMap_[msg] != nullptr)
		return evnetHandlerMap_[msg](param);
	if (defaultEventHandler_ != nullptr)
		return defaultEventHandler_(param);
	return 0;
}

Listener& Listener::WinProcMsgListener() {
	static Listener l;
	return l;
}
