#include "stdafx.h"
#include "WM_Command.h"
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
	DialogBox(Listener::WinProcMsgListener().hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), param.hWnd_, About);
	return 0;
}

// [關於] 方塊的訊息處理常式。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}