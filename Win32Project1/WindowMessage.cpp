#include "stdafx.h"
#include "WindowMessage.h"
#include "Listener.h"
#include "Resource.h"

LRESULT DefaultEvnetHandler(Parameter& param)
{
	TCHAR szBuffer[50];
	wsprintf(szBuffer, TEXT("Button ID %d : %d"), param.wParam_, param.lParam_);
	MessageBox(param.hWnd_, szBuffer, TEXT("Pressed"), MB_OK);
	return 0;//DefWindowProc(param.hWnd_, param.message_, param.wParam_, param.lParam_);
}

LRESULT AboutEvent(Parameter& param)
{
	DialogBox(Listener::MyWinProcMessageListener().hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), param.hWnd_, About);
	return 0;
}