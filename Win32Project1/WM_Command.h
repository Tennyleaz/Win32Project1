#pragma once
#include "stdafx.h"
#include "Listener.h"

LRESULT DefaultEvnetHandler(Parameter& param);

LRESULT AboutEvent(Parameter& param);

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);