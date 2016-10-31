#pragma once
#include "stdafx.h"
#include <unordered_map>
#include <functional>


struct Parameter
{
	HWND hWnd_;
	WPARAM wParam_;
	LPARAM lParam_;
	Parameter(HWND hWnd, WPARAM wParam, LPARAM lParam)
		:hWnd_(hWnd)
		, wParam_(wParam)
		, lParam_(lParam)
	{}
};

using Event = std::function<LRESULT(Parameter& param)>;

class Listener
{
private:
	std::unordered_map<UINT, Event> evnetHandlerMap_;
	Event defaultEventHandler_;
public:
	static Listener& MyWinProcMessageListener() {
		static Listener l;
		return l;
	}
	inline void AddDefaultEvent(Event e) { defaultEventHandler_ = e; }
	inline void AddEvent(UINT msg, Event e) { evnetHandlerMap_[msg] = e; }
	LRESULT Trig(UINT msg, Parameter& param);
};