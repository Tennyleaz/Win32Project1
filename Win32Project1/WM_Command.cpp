#include "stdafx.h"
#include "WM_Command.h"
#include "Resource.h"
#include "globals.h"
#include "Save.h"
#include "Win32Project1.h"

LRESULT DefaultEvnetHandler(Parameter& param)
{
	TCHAR szBuffer[50];
	wsprintf(szBuffer, TEXT("Button ID %d : %d"), param.wParam_, param.lParam_);
	MessageBox(param.hWnd_, szBuffer, TEXT("Pressed"), MB_OK);
	return 0;//DefWindowProc(param.hWnd_, param.message_, param.wParam_, param.lParam_);
}

LRESULT WM_CommandEvent(Parameter& param)
{
	int wmId = LOWORD(param.wParam_);
	// 剖析功能表選取項目: 
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), param.hWnd_, About);
		break;
	case IDM_EXIT:
		if (modifyState == 1)
		{
			if (DisplayConfirmNewFileMessageBox(fileName) == IDYES)
				SaveToFile(DrawObjList, fileName);
		}
		DestroyWindow(param.hWnd_);
		break;
	case ID_COMMAND_1:  //清除
		if (modifyState == 1)
		{
			if (DisplayConfirmClearMessageBox(fileName) == IDYES)
				SaveToFile(DrawObjList, fileName);
		}
		modifyState = 1;
		newLine.clean();
		newRect.clean();
		newText.clean();
		newCircle.clean();
		selectedObject = NULL;
		currentCursorMode = 0;
		hasSelected = false;
		CleanObjects(param.hWnd_);
		break;
	case 120:
		SetFocus(param.hWnd_);  //return the focus back to main window
	case ID_LineTool:
		currentDrawMode = 0;
		ChangeToolsSelectionState(currentDrawMode, hMenu);
		PushCurrentNewText(newText);
		hasSelected = false;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case 121:
		SetFocus(param.hWnd_);
	case ID_RectTool:
		currentDrawMode = 1;
		ChangeToolsSelectionState(currentDrawMode, hMenu);
		PushCurrentNewText(newText);
		hasSelected = false;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case 122:
		SetFocus(param.hWnd_);
	case ID_CircleTool:
		currentDrawMode = 2;
		ChangeToolsSelectionState(currentDrawMode, hMenu);
		PushCurrentNewText(newText);
		hasSelected = false;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case 123:
		SetFocus(param.hWnd_);
	case ID_TextTool:
		currentDrawMode = 3;
		hasSelected = false;
		ChangeToolsSelectionState(currentDrawMode, hMenu);
		break;
	case 124:
		SetFocus(param.hWnd_);
	case ID_SelectTool:
		currentDrawMode = 4;
		PushCurrentNewText(newText);
		ChangeToolsSelectionState(currentDrawMode, hMenu);
		break;
	case ID_BLACK:
		currentColor = 0;
		newText.color = currentColor;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case ID_GRAY:
		currentColor = 1;
		ChangeColorsSelectionState(currentColor, hMenu);
		newText.color = currentColor;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case ID_RED:
		currentColor = 2;
		ChangeColorsSelectionState(currentColor, hMenu);
		newText.color = currentColor;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case ID_GREEN:
		currentColor = 3;
		ChangeColorsSelectionState(currentColor, hMenu);
		newText.color = currentColor;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case ID_BLU:
		currentColor = 4;
		ChangeColorsSelectionState(currentColor, hMenu);
		newText.color = currentColor;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case ID_CYAN:
		currentColor = 5;
		ChangeColorsSelectionState(currentColor, hMenu);
		newText.color = currentColor;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case ID_YELLOW:
		currentColor = 6;
		ChangeColorsSelectionState(currentColor, hMenu);
		newText.color = currentColor;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case ID_Magenta:
		currentColor = 7;
		ChangeColorsSelectionState(currentColor, hMenu);
		newText.color = currentColor;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case ID_BG_Transparent:
		currentBgColor = 0;
		break;
	case  ID_BG_GRAY:
		currentBgColor = 1;
		break;
	case  ID_BG_RED:
		currentBgColor = 2;
		break;
	case  ID_BG_GREEN:
		currentBgColor = 3;
		break;
	case  ID_BG_BLU:
		currentBgColor = 4;
		break;
	case  ID_BG_CYAN:
		currentBgColor = 5;
		break;
	case  ID_BG_YELLOW:
		currentBgColor = 6;
		break;
	case  ID_BG_Magenta:
		currentBgColor = 7;
		break;
	case ID_Wide1:
		currentLineWidth = 1;
		break;
	case ID_Wide2:
		currentLineWidth = 2;
		break;
	case ID_Wide3:
		currentLineWidth = 3;
		break;
	case ID_Wide4:
		currentLineWidth = 4;
		break;
	case ID_Wide5:
		currentLineWidth = 5;
		break;
	case ID_SAVE:
		if (modifyState == 0 || lastFilePath == NULL || wcslen(lastFilePath) < 1)
			goto SAVE_AS_NEW_FILE;
		PushCurrentNewText(newText);
		SaveToLastFilePath(DrawObjList);
		modifyState = 2;
		break;
	case ID_SAVE_AS:
	{
	SAVE_AS_NEW_FILE:
		PushCurrentNewText(newText);
		SaveToFile(DrawObjList, fileName);
		SetTitle(fileName, param.hWnd_);
		modifyState = 2;
		break;
	}
	case ID_NEW_FILE:
		if (modifyState == 1)
		{
			if (DisplayConfirmNewFileMessageBox(fileName) == IDYES)
				SaveToFile(DrawObjList, fileName);
		}
		modifyState = 0;
		fileName = "Untitled";
		lastFilePath = NULL;
		SetTitle(fileName, param.hWnd_);
		newLine.clean();
		newRect.clean();
		newText.clean();
		newCircle.clean();
		selectedObject = NULL;
		currentCursorMode = 0;
		hasSelected = false;
		CleanObjects(param.hWnd_);
		break;
	case ID_OPEN_FILE:
	{
		//cleanObjects(param.hWnd_);
		ReadFromFile(DrawObjList, fileName);
		newLine.clean();
		newRect.clean();
		newText.clean();
		newCircle.clean();
		SetTitle(fileName, param.hWnd_);
		InvalidateRect(param.hWnd_, NULL, FALSE);
		modifyState = 2;
		break;
	}
	default:
		wsprintf(szBuffer, TEXT("Button ID %d : %d"), param.wParam_, param.lParam_);
		MessageBox(param.hWnd_, szBuffer, TEXT("Pressed"), MB_OK);
		return DefWindowProc(param.hWnd_, param.message_, param.wParam_, param.lParam_);
	}
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