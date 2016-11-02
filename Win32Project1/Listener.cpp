#include "stdafx.h"
#include "Listener.h"

LRESULT Listener::Trig(UINT msg, Parameter& param)
{
	if (evnetHandlerMap_[msg] != nullptr)
		return evnetHandlerMap_[msg](param);
	//if (defaultEventHandler_ != nullptr)
	//	return defaultEventHandler_(param);
	return DefWindowProc(param.hWnd_, msg, param.wParam_, param.lParam_);
}

Listener& Listener::WinProcMsgListener() {
	static Listener l;
	return l;
}
