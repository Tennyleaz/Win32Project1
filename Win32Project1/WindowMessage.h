#pragma once
#include "stdafx.h"

LRESULT DefaultEvnetHandler(Parameter& param)
{
	TCHAR szBuffer[50];
	wsprintf(szBuffer, TEXT("Button ID %d : %d"), param.wParam_, param.lParam_);
	MessageBox(param.hWnd_, szBuffer, TEXT("Pressed"), MB_OK);
	return DefWindowProc(param.hWnd_, param.message_, param.wParam_, param.lParam_);
}