#include "stdafx.h"
#include "Windowsx.h "
#include "WM_Command.h"
#include "Resource.h"
#include "globals.h"
#include "Save.h"
#include "Win32Project1.h"
#include "mylog.h"

static int mouseX, mouseY;
static LineObj  newLine;
static RectangularObj newRect;
//static TextObj newText;  //only newText needs to be global now
static CircleObj newCircle;
static bool mouseHasDown = false;

static RECT rect;
static TCHAR szBuffer[50];
static int cxChar, cyChar;
static int xPasteDir, yPasteDir;  //0=負 1=正
//static bool canRedo;
//int i;

SCROLLINFO si;
// These variables are required by BitBlt.
//static HDC hdcWin;           // window DC
static HDC hdcScreen;        // DC for entire screen
///static HDC hdcScreenCompat;  // memory DC for screen
///static HBITMAP hbmpCompat;   // bitmap handle to old DC
static BITMAP bmp;           // bitmap data structure
static BOOL fBlt;            // TRUE if BitBlt occurred
static BOOL fScroll;         // TRUE if scrolling occurred
static BOOL fSize;           // TRUE if fBlt & WM_SIZE

// These variables are required for horizontal scrolling.
static int xMinScroll;       // minimum horizontal scroll value
static int xCurrentScroll;   // current horizontal scroll value
static int xMaxScroll;       // maximum horizontal scroll value

// These variables are required for vertical scrolling.
static int yMinScroll;       // minimum vertical scroll value
static int yCurrentScroll;   // current vertical scroll value
static int yMaxScroll;       // maximum vertical scroll value
//static HBITMAP bmpIcon1, bmpIcon2, bmpIcon3, bmpIcon4, bmpIcon5, bmpIcon6, bmpIcon7, bmpIcon8, bmpIcon9;   //a bitmap icon for button
static HBITMAP checkedIcon;
static HMENU hMenu=NULL;        //try to get the system menu
static HBITMAP hBmp;          //bitmap for memory DC

static int currentColor;        //0=black, 0~7 kinds of color
static int currentBgColor;      //0=transparent, 0~7 kinds of color
static int currentLineWidth;    //width 1~5
static int currentCursorMode;   //0=original 1=左上 2=右下 3=右上 4=左下 5=左 6=右 7=上 8=下 9=四向
//static bool hasSelected;
static HCURSOR cursors[6];      //0=original 1=左上右下 2=右上左下 3=左右 4=上下 5=四向
static bool isMoving, isResizing;
//string debugmessage = "cursorX=";
static int hScrollResize = 0;
static HCURSOR * currentCursor;

using json = nlohmann::json;

LRESULT DefaultEvnetHandler(Parameter& param)
{
	/*TCHAR szBuffer[50];
	wsprintf(szBuffer, TEXT("Button ID %d : %d"), param.wParam_, param.lParam_);
	MessageBox(param.hWnd_, szBuffer, TEXT("Pressed"), MB_OK);
	return 0*/
	return DefWindowProc(param.hWnd_, param.message_, param.wParam_, param.lParam_);
}

LRESULT WM_CommandEvent(Parameter& param)
{
	int wmId = LOWORD(param.wParam_);
	// 剖析功能表選取項目: 
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(globals::var().hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), param.hWnd_, About);
		break;
	case IDM_EXIT:
		SendMessage(param.hWnd_, WM_CLOSE, NULL, NULL);	
		break;
		//if (globals::var().modifyState == 1)
		//{
		//	if (DisplayConfirmNewFileMessageBox(globals::var().fileName) == IDYES)
		//	{
		//		PushCurrentNewText(globals::var().newText);
		//		if (globals::var().lastFilePath.size() < 1)
		//		{
		//			SaveToFile(globals::var().DrawObjList, globals::var().fileName);
		//			SetTitle(globals::var().fileName, param.hWnd_);
		//			globals::var().modifyState = 2;
		//		}
		//		else
		//		{
		//			SaveToLastFilePath(globals::var().DrawObjList);
		//			globals::var().modifyState = 2;
		//		}
		//	}
		//}
		DestroyWindow(param.hWnd_);
		break;
	case ID_COMMAND_1:  //清除
	{
		if (globals::var().modifyState == 1)
		{
			if (DisplayConfirmClearMessageBox(globals::var().fileName) == IDYES)
			{
				PushCurrentNewText(globals::var().newText);
				if (globals::var().lastFilePath.size() < 1)
				{
					SaveToFile(globals::var().DrawObjList, globals::var().fileName);
					SetTitle(globals::var().fileName, param.hWnd_);
					globals::var().modifyState = 2;
				}
				else
				{
					SaveToLastFilePath(globals::var().DrawObjList);
					globals::var().modifyState = 2;
				}
			}
		}
		int oldState = globals::var().modifyState;
		globals::var().mlog.ClearLogs();
		if (globals::var().DrawObjList.size() > 0)
			globals::var().modifyState = 1;
		else
			globals::var().modifyState = oldState;
		newLine.clean();
		newRect.clean();
		globals::var().newText.clean();
		newCircle.clean();
		globals::var().selectedObjectPtr = NULL;
		//globals::var().currentDrawMode = 0;
		currentCursorMode = 0;
		globals::var().hasSelected = false;
		CleanObjects(param.hWnd_);
		break;
	}
	case 120:
		SetFocus(param.hWnd_);  //return the focus back to main window
	case ID_LineTool:
		globals::var().currentDrawMode = 0;
		ChangeToolsSelectionState(globals::var().currentDrawMode, hMenu);
		PushCurrentNewText(globals::var().newText);
		globals::var().hasSelected = false;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case 121:
		SetFocus(param.hWnd_);
	case ID_RectTool:
		globals::var().currentDrawMode = 1;
		ChangeToolsSelectionState(globals::var().currentDrawMode, hMenu);
		PushCurrentNewText(globals::var().newText);
		globals::var().hasSelected = false;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case 122:
		SetFocus(param.hWnd_);
	case ID_CircleTool:
		globals::var().currentDrawMode = 2;
		ChangeToolsSelectionState(globals::var().currentDrawMode, hMenu);
		PushCurrentNewText(globals::var().newText);
		globals::var().hasSelected = false;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case 123:
		SetFocus(param.hWnd_);
	case ID_TextTool:
		globals::var().currentDrawMode = 3;
		globals::var().hasSelected = false;
		ChangeToolsSelectionState(globals::var().currentDrawMode, hMenu);
		break;
	case 124:
		SetFocus(param.hWnd_);
	case ID_SelectTool:
		globals::var().currentDrawMode = 4;
		PushCurrentNewText(globals::var().newText);
		ChangeToolsSelectionState(globals::var().currentDrawMode, hMenu);
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case ID_BLACK:
		currentColor = 0;		
		globals::var().newText.color = currentColor;
		ChangeColorsSelectionState(currentColor, hMenu);
		break;
	case ID_GRAY:
		currentColor = 1;		
		globals::var().newText.color = currentColor;
		ChangeColorsSelectionState(currentColor, hMenu);
		break;
	case ID_RED:
		currentColor = 2;
		globals::var().newText.color = currentColor;
		ChangeColorsSelectionState(currentColor, hMenu);
		break;
	case ID_GREEN:
		currentColor = 3;
		globals::var().newText.color = currentColor;
		ChangeColorsSelectionState(currentColor, hMenu);
		break;
	case ID_BLU:
		currentColor = 4;
		globals::var().newText.color = currentColor;
		ChangeColorsSelectionState(currentColor, hMenu);
		break;
	case ID_CYAN:
		currentColor = 5;
		globals::var().newText.color = currentColor;
		ChangeColorsSelectionState(currentColor, hMenu);
		break;
	case ID_YELLOW:
		currentColor = 6;
		globals::var().newText.color = currentColor;
		ChangeColorsSelectionState(currentColor, hMenu);
		break;
	case ID_Magenta:
		currentColor = 7;
		globals::var().newText.color = currentColor;
		ChangeColorsSelectionState(currentColor, hMenu);
		break;
	case ID_BG_Transparent:
		currentBgColor = 0;
		ChangeBGSelectionState(currentBgColor, hMenu);
		break;
	case  ID_BG_GRAY:
		currentBgColor = 1;
		ChangeBGSelectionState(currentBgColor, hMenu);
		break;
	case  ID_BG_RED:
		currentBgColor = 2;
		ChangeBGSelectionState(currentBgColor, hMenu);
		break;
	case  ID_BG_GREEN:
		currentBgColor = 3;
		ChangeBGSelectionState(currentBgColor, hMenu);
		break;
	case  ID_BG_BLU:
		currentBgColor = 4;
		ChangeBGSelectionState(currentBgColor, hMenu);
		break;
	case  ID_BG_CYAN:
		currentBgColor = 5;
		ChangeBGSelectionState(currentBgColor, hMenu);
		break;
	case  ID_BG_YELLOW:
		currentBgColor = 6;
		ChangeBGSelectionState(currentBgColor, hMenu);
		break;
	case  ID_BG_Magenta:
		currentBgColor = 7;
		ChangeBGSelectionState(currentBgColor, hMenu);
		break;
	case ID_Wide1:
		currentLineWidth = 1;
		ChangeLineSelectionState(currentLineWidth, hMenu);
		break;
	case ID_Wide2:
		currentLineWidth = 2;
		ChangeLineSelectionState(currentLineWidth, hMenu);
		break;
	case ID_Wide3:
		currentLineWidth = 3;
		ChangeLineSelectionState(currentLineWidth, hMenu);
		break;
	case ID_Wide4:
		currentLineWidth = 4;
		ChangeLineSelectionState(currentLineWidth, hMenu);
		break;
	case ID_Wide5:
		currentLineWidth = 5;
		ChangeLineSelectionState(currentLineWidth, hMenu);
		break;
	case ID_SAVE:
		if (globals::var().modifyState == 0 || globals::var().lastFilePath.size() <1)
			goto SAVE_AS_NEW_FILE;
		PushCurrentNewText(globals::var().newText);
		SaveToLastFilePath(globals::var().DrawObjList);
		globals::var().modifyState = 2;
		SetTitle(globals::var().fileName, param.hWnd_);
		break;
	case ID_SAVE_AS:
	{
	SAVE_AS_NEW_FILE:
		PushCurrentNewText(globals::var().newText);
		SaveToFile(globals::var().DrawObjList, globals::var().fileName);		
		globals::var().modifyState = 2;
		SetTitle(globals::var().fileName, param.hWnd_);
		break;
	}
	case ID_NEW_FILE:
		if (globals::var().modifyState == 1)
		{
			if (DisplayConfirmNewFileMessageBox(globals::var().fileName) == IDYES)
			{
				PushCurrentNewText(globals::var().newText);
				if (globals::var().modifyState == 0 || globals::var().lastFilePath.size() < 1)
					SaveToFile(globals::var().DrawObjList, globals::var().fileName);  //do not have last opened file					
				else
					SaveToLastFilePath(globals::var().DrawObjList);  //save to last opened file
			}
		}
		globals::var().modifyState = 0;
		globals::var().fileName = "Untitled";
		globals::var().lastFilePath.clear();
		SetTitle(globals::var().fileName, param.hWnd_);
		newLine.clean();
		newRect.clean();
		globals::var().newText.clean();
		newCircle.clean();
		globals::var().selectedObjectPtr = NULL;
		//globals::var().currentDrawMode = 0;
		currentCursorMode = 0;
		globals::var().hasSelected = false;
		CleanObjects(param.hWnd_);
		globals::var().mlog.ClearLogs();
		break;
	case ID_OPEN_FILE:
	{
		//cleanObjects(param.hWnd_);
		globals::var().selectedObjectPtr = NULL;
		//globals::var().currentDrawMode = 0;
		globals::var().hasSelected = false;
		currentCursorMode = 0;
		globals::var().mlog.ClearLogs();
		CleanObjects(param.hWnd_);
		ReadFromFile(globals::var().DrawObjList, globals::var().fileName);
		//InvalidateRect(param.hWnd_, NULL, FALSE);
		newLine.clean();
		newRect.clean();
		globals::var().newText.clean();
		newCircle.clean();
		globals::var().modifyState = 2;
		SetTitle(globals::var().fileName, param.hWnd_);		
		InvalidateRect(param.hWnd_, NULL, TRUE);
		break;
	}
	case ID_Copy:
	{
		if(globals::var().selectedObjectPtr == nullptr)
			break;

		if (globals::var().pastebinObjectPtr != nullptr)
			delete globals::var().pastebinObjectPtr;

		xPasteDir = 0;
		yPasteDir = 0;

		switch (globals::var().selectedObjectPtr->objectType)
		{
		case 1:
		{
			LineObj *temp = (LineObj*)globals::var().selectedObjectPtr;
			globals::var().pastebinObjectPtr = new LineObj(*temp);
			break;
		}
		case 2:
		{
			RectangularObj *temp = (RectangularObj*)globals::var().selectedObjectPtr;
			globals::var().pastebinObjectPtr = new RectangularObj(*temp);
			break;
		}
		case 3:
		{
			CircleObj *temp = (CircleObj*)globals::var().selectedObjectPtr;
			globals::var().pastebinObjectPtr = new CircleObj(*temp);
			break;
		}
		case 4:
		{
			TextObj *temp = (TextObj*)globals::var().selectedObjectPtr;
			globals::var().pastebinObjectPtr = new TextObj(*temp);
			break;
		}
		default:
			break;
		}
		break;
	}
	case ID_Cut:
	{
		if (globals::var().selectedObjectPtr == nullptr)
			break;

		if (globals::var().pastebinObjectPtr != nullptr)
			delete globals::var().pastebinObjectPtr;

		xPasteDir = 0;
		yPasteDir = 0;

		switch (globals::var().selectedObjectPtr->objectType)
		{
		case 1:
		{
			LineObj *temp = (LineObj*)globals::var().selectedObjectPtr;
			globals::var().pastebinObjectPtr = new LineObj(*temp);
			break;
		}
		case 2:
		{
			RectangularObj *temp = (RectangularObj*)globals::var().selectedObjectPtr;
			globals::var().pastebinObjectPtr = new RectangularObj(*temp);
			break;
		}
		case 3:
		{
			CircleObj *temp = (CircleObj*)globals::var().selectedObjectPtr;
			globals::var().pastebinObjectPtr = new CircleObj(*temp);
			break;
		}
		case 4:
		{
			TextObj *temp = (TextObj*)globals::var().selectedObjectPtr;
			globals::var().pastebinObjectPtr = new TextObj(*temp);
			break;
		}
		default:
			break;
		}

		//find the position of the selected object in list
		auto it = std::find(globals::var().DrawObjList.begin(), globals::var().DrawObjList.end(), globals::var().selectedObjectPtr);
		if (it != globals::var().DrawObjList.end())
		{
			delete globals::var().selectedObjectPtr;
			globals::var().hasSelected = false;
			globals::var().selectedObjectPtr = nullptr;
			currentCursorMode = 0;
			globals::var().DrawObjList.erase(it);
			InvalidateRect(param.hWnd_, NULL, FALSE);
		}
		break;
	}
	case ID_Paste:
	{
		if (globals::var().pastebinObjectPtr == nullptr)
			break;

		//adjust new paste position
		POINT p = MovePastedObj();
		AutoScroll(param.hWnd_, p.x- xCurrentScroll+1, p.y- yCurrentScroll+1, xCurrentScroll, yCurrentScroll, rect);

		//insert the pastbin object into list
		switch (globals::var().pastebinObjectPtr->objectType)
		{
			case 1:
			{
				globals::var().DrawObjList.push_back(new LineObj(*(LineObj*)globals::var().pastebinObjectPtr));
				break;
			}
			case 2:
			{
				globals::var().DrawObjList.push_back(new RectangularObj(*(RectangularObj*)globals::var().pastebinObjectPtr));
				break;
			}
			case 3:
			{
				globals::var().DrawObjList.push_back(new CircleObj(*(CircleObj*)globals::var().pastebinObjectPtr));
				break;
			}
			case 4:
			{
				globals::var().DrawObjList.push_back(new TextObj(*(TextObj*)globals::var().pastebinObjectPtr));
				break;
			}
			default:
				break;
		}

		//select the new object
		globals::var().selectedObjectPtr = globals::var().DrawObjList.back();
		globals::var().hasSelected = true;

		InvalidateRect(param.hWnd_, NULL, FALSE);  
		break;
	}
	case ID_Undo:
	{
		Undo();
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	}
	case ID_Redo:
	{
		Redo();
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	}
	case ID_Delete:
	{
		/*//do nothing wheh mode = 0~2
		if (globals::var().currentDrawMode < 3)
			break;*/

		//if editing, goto WM_KeyDownEvent
		if (globals::var().currentDrawMode == 3 && globals::var().newText.startFinished) //delete the newText
		{
			//LPARAM lparam = 0x00000001;
			////WPARAM wParam;
			////wParam = MAKEWPARAM(VK_DELETE, VK_DELETE);
			//SendMessage(globals::var().hWndFather, WM_KEYDOWN, VK_DELETE, lparam);
			
			//record the log
			globals::var().mlog.OP_del(&globals::var().newText, -1);

			globals::var().newText.clean();

			InvalidateRect(param.hWnd_, NULL, FALSE);
			break;
		}

		/*if (globals::var().selectedObjectPtr != nullptr && globals::var().selectedObjectPtr->objectType == 4 && globals::var().hasSelected)
		{
			LPARAM lparam = 0x00000001;
			SendMessage(globals::var().hWndFather, WM_KEYDOWN, VK_DELETE, lparam);
			break;
		}*/

		if(globals::var().selectedObjectPtr == nullptr)
			break;

		//find the position of the selected object in list
		auto it = std::find(globals::var().DrawObjList.begin(), globals::var().DrawObjList.end(), globals::var().selectedObjectPtr);
		if (it != globals::var().DrawObjList.end())
		{
			//record the log
			int pos = distance(globals::var().DrawObjList.begin(), it);
			globals::var().mlog.OP_del(*it, pos);

			//doing delete
			delete globals::var().selectedObjectPtr;
			globals::var().DrawObjList.erase(it);

			//select the previous object
			it = globals::var().DrawObjList.begin();
			if (globals::var().DrawObjList.size() > 0 && *it != NULL)
			{
				if (pos > 0)
					advance(it, pos - 1);
				globals::var().selectedObjectPtr = *it;
			}
			else
			{
				globals::var().selectedObjectPtr = NULL;
				globals::var().hasSelected = false;
			}
			currentCursorMode = 0;

			InvalidateRect(param.hWnd_, NULL, FALSE);
		}
		break;
	}
	default:
		wsprintf(szBuffer, TEXT("Button ID %d : %d"), param.wParam_, param.lParam_);
		MessageBox(param.hWnd_, szBuffer, TEXT("Pressed"), MB_OK);
		return DefWindowProc(param.hWnd_, param.message_, param.wParam_, param.lParam_);
	}
	return 0;
}

LRESULT WM_CreateEvent(Parameter& param)
{
	currentColor = 0;
	globals::var().hasSelected = false;
	cxChar = LOWORD(GetDialogBaseUnits());
	cyChar = HIWORD(GetDialogBaseUnits());

	HBITMAP bmpIcon1, bmpIcon2, bmpIcon3, bmpIcon4, bmpIcon5, bmpIcon6, bmpIcon7, bmpIcon8, bmpIcon9;
	HBITMAP line1, line2, line3, line4, line5;
	//image for child window buttons
	bmpIcon1 = (HBITMAP)LoadImage(NULL, L"black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon2 = (HBITMAP)LoadImage(NULL, L"grey.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon3 = (HBITMAP)LoadImage(NULL, L"red.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon4 = (HBITMAP)LoadImage(NULL, L"green.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon5 = (HBITMAP)LoadImage(NULL, L"blu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon6 = (HBITMAP)LoadImage(NULL, L"cyan.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon7 = (HBITMAP)LoadImage(NULL, L"yellow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon8 = (HBITMAP)LoadImage(NULL, L"magenta.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon9 = (HBITMAP)LoadImage(NULL, L"transparent.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	checkedIcon = (HBITMAP)LoadImage(NULL, L"checkedIcon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	line1 = (HBITMAP)LoadImage(NULL, L"wide1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	line2 = (HBITMAP)LoadImage(NULL, L"wide2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	line3 = (HBITMAP)LoadImage(NULL, L"wide3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	line4 = (HBITMAP)LoadImage(NULL, L"wide4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	line5 = (HBITMAP)LoadImage(NULL, L"wide5.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//add image to menu
	hMenu = GetMenu(param.hWnd_); //LoadMenu(NULL, MAKEINTRESOURCE(IDC_WIN32PROJECT1));
	if (hMenu)
	{
		//ModifyMenu(hMenu, ID_COMMAND_1, MF_BYCOMMAND | MF_STRING, ID_COMMAND_1, (PCTSTR)(LONG)bmpIcon1);
		HMENU hMenu2 = GetSubMenu(hMenu, 2);   //hMenu2 = 工具
		HMENU hMenu3 = GetSubMenu(hMenu2, 6);  //hMenu3 = 顏色
		HMENU hMenu4 = GetSubMenu(hMenu2, 7);  //hMenu4 = 線寬
		HMENU hMenu5 = GetSubMenu(hMenu2, 8);  //hMenu5 = 底色

		ModifyMenu(hMenu3, 0, MF_BYPOSITION | MF_BITMAP, ID_BLACK, (LPCTSTR)bmpIcon1);
		ModifyMenu(hMenu3, 1, MF_BYPOSITION | MF_BITMAP, ID_GRAY, (LPCTSTR)bmpIcon2);
		ModifyMenu(hMenu3, 2, MF_BYPOSITION | MF_BITMAP, ID_RED, (LPCTSTR)bmpIcon3);
		ModifyMenu(hMenu3, 3, MF_BYPOSITION | MF_BITMAP, ID_GREEN, (LPCTSTR)bmpIcon4);
		ModifyMenu(hMenu3, 4, MF_BYPOSITION | MF_BITMAP, ID_BLU, (LPCTSTR)bmpIcon5);
		ModifyMenu(hMenu3, 5, MF_BYPOSITION | MF_BITMAP, ID_CYAN, (LPCTSTR)bmpIcon6);
		ModifyMenu(hMenu3, 6, MF_BYPOSITION | MF_BITMAP, ID_YELLOW, (LPCTSTR)bmpIcon7);
		ModifyMenu(hMenu3, 7, MF_BYPOSITION | MF_BITMAP, ID_Magenta, (LPCTSTR)bmpIcon8);
		ModifyMenu(hMenu5, 0, MF_BYPOSITION | MF_BITMAP, ID_BG_Transparent, (LPCTSTR)bmpIcon9);
		ModifyMenu(hMenu5, 1, MF_BYPOSITION | MF_BITMAP, ID_BG_GRAY, (LPCTSTR)bmpIcon2);
		ModifyMenu(hMenu5, 2, MF_BYPOSITION | MF_BITMAP, ID_BG_RED, (LPCTSTR)bmpIcon3);
		ModifyMenu(hMenu5, 3, MF_BYPOSITION | MF_BITMAP, ID_BG_GREEN, (LPCTSTR)bmpIcon4);
		ModifyMenu(hMenu5, 4, MF_BYPOSITION | MF_BITMAP, ID_BG_BLU, (LPCTSTR)bmpIcon5);
		ModifyMenu(hMenu5, 5, MF_BYPOSITION | MF_BITMAP, ID_BG_CYAN, (LPCTSTR)bmpIcon6);
		ModifyMenu(hMenu5, 6, MF_BYPOSITION | MF_BITMAP, ID_BG_YELLOW, (LPCTSTR)bmpIcon7);
		ModifyMenu(hMenu5, 7, MF_BYPOSITION | MF_BITMAP, ID_BG_Magenta, (LPCTSTR)bmpIcon8);
		ModifyMenu(hMenu4, 0, MF_BYPOSITION | MF_BITMAP, ID_Wide1, (LPCTSTR)line1);
		ModifyMenu(hMenu4, 1, MF_BYPOSITION | MF_BITMAP, ID_Wide2, (LPCTSTR)line2);
		ModifyMenu(hMenu4, 2, MF_BYPOSITION | MF_BITMAP, ID_Wide3, (LPCTSTR)line3);
		ModifyMenu(hMenu4, 3, MF_BYPOSITION | MF_BITMAP, ID_Wide4, (LPCTSTR)line4);
		ModifyMenu(hMenu4, 4, MF_BYPOSITION | MF_BITMAP, ID_Wide5, (LPCTSTR)line5);
		//DeleteObject(line1);
		CheckMenuItem(hMenu3, ID_BLACK, MF_CHECKED);
		CheckMenuItem(hMenu4, ID_Wide1, MF_CHECKED);
		CheckMenuItem(hMenu5, ID_BG_Transparent, MF_CHECKED);
		for (int i = 0; i < 8; i++)
		{
			SetMenuItemBitmaps(hMenu3, i, MF_BYPOSITION, NULL, checkedIcon);
			SetMenuItemBitmaps(hMenu5, i, MF_BYPOSITION, NULL, checkedIcon);
			if (i < 5)
				SetMenuItemBitmaps(hMenu4, i, MF_BYPOSITION, NULL, checkedIcon);
		}

		//bmpIcon1 = (HBITMAP)LoadImage(NULL, L"grey.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//bmpIcon2 = (HBITMAP)LoadImage(NULL, L"greychecked.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		/*SetMenuItemBitmaps(hMenu3, 0, MF_BYPOSITION, bmpIcon1, bmpIcon1);
		SetMenuItemBitmaps(hMenu3, 1, MF_BYPOSITION, bmpIcon2, bmpIcon2);
		SetMenuItemBitmaps(hMenu3, 2, MF_BYPOSITION, bmpIcon3, bmpIcon3);
		SetMenuItemBitmaps(hMenu3, 3, MF_BYPOSITION, bmpIcon4, bmpIcon4);
		SetMenuItemBitmaps(hMenu3, 4, MF_BYPOSITION, bmpIcon5, bmpIcon5);
		SetMenuItemBitmaps(hMenu3, 5, MF_BYPOSITION, bmpIcon6, bmpIcon6);
		SetMenuItemBitmaps(hMenu3, 6, MF_BYPOSITION, bmpIcon7, bmpIcon7);
		SetMenuItemBitmaps(hMenu3, 7, MF_BYPOSITION, bmpIcon8, bmpIcon8);*/
		/*AppendMenu(hMenu3, MF_BITMAP, 111, (PCTSTR)(LONG)bmpIcon1);
		CheckMenuItem(hMenu3, 111, MF_CHECKED);*/
	}
	else
		MessageBox(param.hWnd_, 0, TEXT("NO MENU"), MB_OK);

	cursors[0] = LoadCursor(NULL, IDC_ARROW);  // 0 = default mouse
	cursors[1] = LoadCursor(NULL, IDC_SIZENWSE);  //1~2 = 四角的游標 1=左上右下
	cursors[2] = LoadCursor(NULL, IDC_SIZENESW);  //2=右上左下
	cursors[3] = LoadCursor(NULL, IDC_SIZEWE);  //3~4 = 邊上的游標 3=左右
	cursors[4] = LoadCursor(NULL, IDC_SIZENS);  //4=上下
	cursors[5] = LoadCursor(NULL, IDC_SIZEALL);
	currentCursor = &cursors[0];

	// Create a normal DC and a memory DC for the entire 
	// screen. The normal DC provides a snapshot of the 
	// screen contents. The memory DC keeps a copy of this 
	// snapshot in the associated bitmap. 
	hdcScreen = CreateDC(L"DISPLAY", (PCTSTR)NULL, (PCTSTR)NULL, (CONST DEVMODE *) NULL);
	///hdcScreenCompat = CreateCompatibleDC(hdcScreen);

	// Retrieve the metrics for the bitmap associated with the 
	// regular device context.  bmp size=1920*1080
	bmp.bmBitsPixel = (BYTE)GetDeviceCaps(hdcScreen, BITSPIXEL);
	bmp.bmPlanes = (BYTE)GetDeviceCaps(hdcScreen, PLANES);
	bmp.bmWidth = 2000;//GetDeviceCaps(hdcScreen, HORZRES);
	bmp.bmHeight = 2000;//GetDeviceCaps(hdcScreen, VERTRES);

						// The width must be byte-aligned. 
	bmp.bmWidthBytes = ((bmp.bmWidth + 15) &~15) / 8;

	// Create a bitmap for the compatible DC. 
	///hbmpCompat = CreateBitmap(bmp.bmWidth, bmp.bmHeight,
	///	bmp.bmPlanes, bmp.bmBitsPixel, (CONST VOID *) NULL);

	// Select the bitmap for the compatible DC. 
	///SelectObject(hdcScreenCompat, hbmpCompat);

	// Initialize the flags. 
	fBlt = FALSE;
	fScroll = FALSE;
	fSize = FALSE;

	// Initialize the horizontal scrolling variables. 
	xMinScroll = 0;
	xCurrentScroll = 0;
	xMaxScroll = 0;

	// Initialize the vertical scrolling variables. 
	yMinScroll = 0;
	yCurrentScroll = 0;
	yMaxScroll = 0;
	
	isMoving = false;
	isResizing = false;

	DeleteDC(hdcScreen);
	return 0;
}

LRESULT WM_MouseMoveEvent(Parameter& param)
{

	//if (globals::var().currentDrawMode != 4 && mouseHasDown && currentCursorMode == 0)  //scroll the scrollbars when mouse out of range
	//{
	//	mouseX = GET_X_LPARAM(param.lParam_);
	//	mouseY = GET_Y_LPARAM(param.lParam_);
	//	AutoScroll(param.hWnd_, mouseX, mouseY, xCurrentScroll, yCurrentScroll, rect);
	//}
	mouseX = GET_X_LPARAM(param.lParam_) + xCurrentScroll;
	mouseY = GET_Y_LPARAM(param.lParam_) + yCurrentScroll;

	if (globals::var().currentDrawMode == 0 && currentCursorMode == 0 && mouseHasDown) //draw the line
	{
		if (!newLine.endFinished && newLine.startFinished)
		{
			newLine.ptEnd.x = mouseX;
			newLine.ptEnd.y = mouseY;			
			if (newLine.ptEnd.x < 0)
				newLine.ptEnd.x = 0;
			else if (newLine.ptEnd.x > 2000)
				newLine.ptEnd.x = 2000;
			if (newLine.ptEnd.y < 0)
				newLine.ptEnd.y = 0;
			else if (newLine.ptEnd.y > 2000)
				newLine.ptEnd.y = 2000;

			AutoScrollObjectWhenDrawing(param.hWnd_, &newLine, xCurrentScroll, yCurrentScroll, rect);
			InvalidateRect(param.hWnd_, NULL, FALSE);
		}
	}
	else if (globals::var().currentDrawMode == 1 && currentCursorMode == 0 && mouseHasDown)
	{
		if (!newRect.endFinished && newRect.startFinished)
		{
			newRect.ptEnd.x = mouseX;
			newRect.ptEnd.y = mouseY;
			if (newRect.ptEnd.x < 0)
				newRect.ptEnd.x = 0;
			else if (newRect.ptEnd.x > 2000)
				newRect.ptEnd.x = 2000;
			if (newRect.ptEnd.y < 0)
				newRect.ptEnd.y = 0;
			else if (newRect.ptEnd.y > 2000)
				newRect.ptEnd.y = 2000;

			AutoScrollObjectWhenDrawing(param.hWnd_, &newRect, xCurrentScroll, yCurrentScroll, rect);
			InvalidateRect(param.hWnd_, NULL, FALSE);
		}
	}
	else if (globals::var().currentDrawMode == 2 && currentCursorMode == 0 && mouseHasDown)
	{
		if (!newCircle.endFinished && newCircle.startFinished)
		{
			newCircle.ptEnd.x = mouseX;
			newCircle.ptEnd.y = mouseY;
			if (newCircle.ptEnd.x < 0)
				newCircle.ptEnd.x = 0;
			else if (newCircle.ptEnd.x > 2000)
				newCircle.ptEnd.x = 2000;
			if (newCircle.ptEnd.y < 0)
				newCircle.ptEnd.y = 0;
			else if (newCircle.ptEnd.y > 2000)
				newCircle.ptEnd.y = 2000;

			AutoScrollObjectWhenDrawing(param.hWnd_, &newCircle, xCurrentScroll, yCurrentScroll, rect);
			InvalidateRect(param.hWnd_, NULL, FALSE);
		}
	}
	else if (globals::var().currentDrawMode == 3)  //text tool
	{
		//if mouse is not down on object, only change the mouse icon
		if (globals::var().hasSelected && !mouseHasDown)
		{
			//draw a double arrow mouse if mouse is on the 8-points
			currentCursorMode = globals::var().selectedObjectPtr->CheckMouseIsOnSizingOpint(mouseX, mouseY);
			//if(currentCursorMode == 0)
			//SetCursor(cursors[(currentCursorMode + 1) / 2]);			
			//else if(currentCursorMode == 1 || currentCursorMode == 2)

			//draw a moving arrow if mouse is on the object
			if (currentCursorMode == 0 && globals::var().selectedObjectPtr->CheckObjectCollision(mouseX, mouseY))
			{
				//SetCursor(cursors[5]);
				currentCursor = &cursors[5];
				currentCursorMode = 9;
			}
			else
				currentCursor = &cursors[(currentCursorMode + 1) / 2];
		}
		else if (globals::var().hasSelected && mouseHasDown)  //if mouse is down on object, then perform move/resize
		{
			if (currentCursorMode == 9)  //perform move
			{
				if (!isMoving)
				{
					//store the old position of ptBeg
					isMoving = true;
					globals::var().mlog.OP_moveStart(&globals::var().newText, -1);  //log the movement of newText
				}
				globals::var().selectedObjectPtr->Moving(mouseX, mouseY);
				AutoScrollObject(param.hWnd_, globals::var().selectedObjectPtr, xCurrentScroll, yCurrentScroll, rect);
				InvalidateRect(param.hWnd_, NULL, FALSE);
			}
			else if (currentCursorMode > 0 && currentCursorMode < 9)  //perform resize
			{
				if (!isResizing)  //log the old size values
				{
					isResizing = true;
					globals::var().mlog.OP_sizeStart(&globals::var().newText, -1);  //log the resize of newText
				}

				if (globals::var().selectedObjectPtr->objectType == 4)
				{
					TextObj * temp;
					temp = (TextObj*)globals::var().selectedObjectPtr;
					temp->ResizingText(mouseX, mouseY, currentCursorMode);
					AutoScrollObject(param.hWnd_, globals::var().selectedObjectPtr, xCurrentScroll, yCurrentScroll, rect);
				}
				else
				{
					globals::var().selectedObjectPtr->Resizing(mouseX, mouseY, currentCursorMode);
					AutoScrollObject(param.hWnd_, globals::var().selectedObjectPtr, xCurrentScroll, yCurrentScroll, rect);
				}
				InvalidateRect(param.hWnd_, NULL, FALSE);
			}
		}
	}
	else  //currentDrawMode == 4
	{
		if (!mouseHasDown && globals::var().currentDrawMode == 4)
		{
			//check mouse & DrawObjList have collision or not (from tail)
			globals::var().preSelectObject = NULL;
			for (auto it = globals::var().DrawObjList.crbegin(); it != globals::var().DrawObjList.crend(); ++it)
			{
				if ((*it)->CheckObjectCollision(mouseX, mouseY))
				{
					globals::var().preSelectObject = (*it);
					//MessageBox(NULL, L"Mouse Is On An Obj!", L"Yes", MB_OK);
					//InvalidateRect(param.hWnd_, NULL, FALSE);
					break;
					//globals::var().hasSelected = true;
					//break;
					//it->PaintMouseOnRect();
				}
				globals::var().preSelectObject = NULL;				
			}
			InvalidateRect(param.hWnd_, NULL, FALSE);
		}

		//if mouse is not down on object, only change the mouse icon
		if (globals::var().hasSelected && !mouseHasDown)
		{
			//draw a double arrow mouse if mouse is on the 8-points
			currentCursorMode = globals::var().selectedObjectPtr->CheckMouseIsOnSizingOpint(mouseX, mouseY);
			//SetCursor(cursors[(currentCursorMode + 1) / 2]);
			

			//draw a moving arrow if mouse is on the object
			if (currentCursorMode == 0 && globals::var().selectedObjectPtr->CheckObjectCollision(mouseX, mouseY))
			{
				//SetCursor(cursors[5]);
				currentCursor = &cursors[5];
				currentCursorMode = 9;
			}
			else
				currentCursor = &cursors[(currentCursorMode + 1) / 2];
		}
		else if (globals::var().hasSelected && mouseHasDown)  //if mouse is down on object, then perform move/resize
		{
			if (currentCursorMode == 9)  //perform move
			{
				if (!isMoving)
				{
					//store the old position of ptBeg
					isMoving = true;

					//find the position of the selected object in list
					auto it = find(globals::var().DrawObjList.begin(), globals::var().DrawObjList.end(), globals::var().selectedObjectPtr);
					if (it != globals::var().DrawObjList.end())
					{
						int pos = distance(globals::var().DrawObjList.begin(), it);
						globals::var().mlog.OP_moveStart(globals::var().selectedObjectPtr, pos);
					}					
				}
				globals::var().selectedObjectPtr->Moving(mouseX, mouseY);
				AutoScrollObject(param.hWnd_, globals::var().selectedObjectPtr, xCurrentScroll, yCurrentScroll, rect);
				InvalidateRect(param.hWnd_, NULL, FALSE);
			}
			else if (currentCursorMode > 0 && currentCursorMode < 9)  //perform resize
			{
				if (!isResizing)  //log the old size values
				{
					isResizing = true;
					auto it = find(globals::var().DrawObjList.begin(), globals::var().DrawObjList.end(), globals::var().selectedObjectPtr);
					if (it != globals::var().DrawObjList.end())
					{
						int pos = distance(globals::var().DrawObjList.begin(), it);
						globals::var().mlog.OP_sizeStart(globals::var().selectedObjectPtr, pos);
					}
				}

				if (globals::var().selectedObjectPtr->objectType == 4)
				{
					TextObj * temp;
					temp = (TextObj*)globals::var().selectedObjectPtr;
					temp->ResizingText(mouseX, mouseY, currentCursorMode);
					//AutoScroll(param.hWnd_, mouseX - xCurrentScroll, mouseY - yCurrentScroll, xCurrentScroll, yCurrentScroll, rect);
					AutoScrollObject(param.hWnd_, globals::var().selectedObjectPtr, xCurrentScroll, yCurrentScroll, rect);
				}
				else
				{
					globals::var().selectedObjectPtr->Resizing(mouseX, mouseY, currentCursorMode);
					//AutoScroll(param.hWnd_, mouseX - xCurrentScroll, mouseY - yCurrentScroll, xCurrentScroll, yCurrentScroll, rect);
					AutoScrollObject(param.hWnd_, globals::var().selectedObjectPtr, xCurrentScroll, yCurrentScroll, rect);
				}
				InvalidateRect(param.hWnd_, NULL, FALSE);
			}
		}
		else 
		{

		}
	}
	//break;
	SetCursor(*currentCursor);
	//SetClassLong(param.hWnd_, -12, (DWORD)*currentCursor);
	return 0;
}

LRESULT WM_LButtonDownEvent(Parameter& param)
{
	if (param.wParam_ && !mouseHasDown)  //if mouse L is down
	{
		SetCapture(param.hWnd_);  //capture mouse even outside window
		mouseX = GET_X_LPARAM(param.lParam_) + xCurrentScroll;
		mouseY = GET_Y_LPARAM(param.lParam_) + yCurrentScroll;
		if (globals::var().currentDrawMode == 0 && currentCursorMode == 0)
		{
			newLine.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
			globals::var().modifyState = 1;
		}
		else if (globals::var().currentDrawMode == 1 && currentCursorMode == 0)
		{
			newRect.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
			globals::var().modifyState = 1;
		}
		else if (globals::var().currentDrawMode == 2 && currentCursorMode == 0)
		{
			newCircle.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
			globals::var().modifyState = 1;
		}
		else if (globals::var().currentDrawMode == 3 && currentCursorMode == 0)
		{
			//if mouse is on the 8 sizing point, start to resize
			if (currentCursorMode != 0)
			{
				mouseHasDown = true;
				globals::var().selectedObjectPtr->StartToMove(mouseX, mouseY);
				return 0;
			}

			if (!globals::var().newText.startFinished) //click to a new text position without previous start
			{
				globals::var().newText.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
				globals::var().newText.ptEnd.x = globals::var().newText.ptBeg.x + 8 * 5 + 1;
				globals::var().newText.ptEnd.y = globals::var().newText.ptBeg.y + 1 * 13 + 1;
			}
			else if (globals::var().newText.startFinished && !globals::var().newText.endFinished)  //push old text to drawObj list
			{
				PushCurrentNewText(globals::var().newText);
				globals::var().newText.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
				globals::var().newText.ptEnd.x = globals::var().newText.ptBeg.x + 8 * 5 + 1;
				globals::var().newText.ptEnd.y = globals::var().newText.ptBeg.y + 1 * 13 + 1;
			}
			if (globals::var().newText.ptBeg.y > 1982)
			{
				globals::var().newText.ptBeg.y = 1982;
				globals::var().newText.ptEnd.y = globals::var().newText.ptBeg.y + 1 * 13 + 1;
			}
			globals::var().modifyState = 1;
			globals::var().selectedObjectPtr = &globals::var().newText;
			globals::var().hasSelected = true;
		}
		else  //mode = 4
		{
			bool b = false;

			//if mouse is on the 8 sizing point, start to resize
			if (currentCursorMode != 0 && globals::var().hasSelected)
			{
				mouseHasDown = true;
				globals::var().selectedObjectPtr->StartToMove(mouseX, mouseY);
				return 0;
			}

			//if no sizing or moving, detect a collision and select an object
			//check mouse & DrawObjList have collision or not (from tail)		
			mouseHasDown = false;
			for (auto it = globals::var().DrawObjList.crbegin(); it != globals::var().DrawObjList.crend(); ++it)
			{
				if ((*it)->CheckObjectCollision(mouseX, mouseY))
				{
					globals::var().selectedObjectPtr = (*it);
					globals::var().selectedObjectPtr->StartToMove(mouseX, mouseY);  //user may want to move when clicked on object
					globals::var().hasSelected = true;
					mouseHasDown = true;
					currentCursorMode = 9;
					b = true;
					break;
				}
			}

			//if no operation, de-select everything
			if (!b)
			{
				globals::var().selectedObjectPtr == nullptr;
				globals::var().hasSelected = false;
			}

			//draw a selected rectangle			
			InvalidateRect(param.hWnd_, NULL, FALSE);
			return 0;
		}
		InvalidateRect(param.hWnd_, NULL, FALSE);
		mouseHasDown = true;
	}
	return 0;
}

LRESULT WM_LButtonUpEvent(Parameter& param)
{
	if (!param.wParam_)
	{
		ReleaseCapture();  //stop capture mouse
		if (globals::var().currentDrawMode == 0 && currentCursorMode == 0 && mouseHasDown)  //finish draw line
		{
			newLine.makeEnd(GET_X_LPARAM(param.lParam_), GET_Y_LPARAM(param.lParam_), xCurrentScroll, yCurrentScroll);

			//if line too short...
			int distancex = (newLine.ptEnd.x - newLine.ptBeg.x) * (newLine.ptEnd.x - newLine.ptBeg.x);
			int distancey = (newLine.ptEnd.y - newLine.ptBeg.y) * (newLine.ptEnd.y - newLine.ptBeg.y);
			int distance = sqrt(abs(distancex - distancey));
			if (distance < 3)
			{
				newLine.clean();
				mouseHasDown = false;
				InvalidateRect(param.hWnd_, NULL, FALSE);
				return 0;
			}

			globals::var().DrawObjList.push_back(new LineObj(newLine));
			globals::var().mlog.OP_add(&newLine);
			globals::var().selectedObjectPtr = globals::var().DrawObjList.back();
			globals::var().hasSelected = true;

			//newLine.startFinished = false;
			newLine.clean();
			//DrawObjList.push_back(move(make_unique<LineObj>(newline)));
		}
		else if (globals::var().currentDrawMode == 1 && currentCursorMode == 0 && mouseHasDown)  //draw rect
		{
			newRect.makeEnd(GET_X_LPARAM(param.lParam_), GET_Y_LPARAM(param.lParam_), xCurrentScroll, yCurrentScroll);

			//if rect is too small...
			if (abs(newRect.ptEnd.x - newRect.ptBeg.x) < 4 && abs(newRect.ptEnd.y - newRect.ptBeg.y) < 4 )
			{
				newRect.clean();
				mouseHasDown = false;
				InvalidateRect(param.hWnd_, NULL, FALSE);
				return 0;
			}

			globals::var().DrawObjList.push_back(new RectangularObj(newRect));
			globals::var().mlog.OP_add(&newRect);
			globals::var().selectedObjectPtr = globals::var().DrawObjList.back();
			globals::var().hasSelected = true;

			newRect.clean();
			//DrawObjList.push_back(move(make_unique<RectangularObj>(newRect)));
		}
		else if (globals::var().currentDrawMode == 2 && currentCursorMode == 0 && mouseHasDown)  //draw circle
		{
			newCircle.makeEnd(GET_X_LPARAM(param.lParam_), GET_Y_LPARAM(param.lParam_), xCurrentScroll, yCurrentScroll);
			
			//if rect is too small...
			if (abs(newCircle.ptEnd.x - newCircle.ptBeg.x) < 4 && abs(newCircle.ptEnd.y - newCircle.ptBeg.y) < 4)
			{
				newCircle.clean();
				mouseHasDown = false;
				InvalidateRect(param.hWnd_, NULL, FALSE);
				return 0;
			}
			globals::var().DrawObjList.push_back(new CircleObj(newCircle));
			globals::var().mlog.OP_add(&newCircle);
			globals::var().selectedObjectPtr = globals::var().DrawObjList.back();
			globals::var().hasSelected = true;

			newCircle.clean();
			//DrawObjList.push_back(move(make_unique<CircleObj>(newCircle)));
		}
		else if (globals::var().currentDrawMode == 3)
		{	
			currentCursorMode = 0;

			//disable the moving flag
			if (isMoving)
			{
				isMoving = false;
				//record the new position of ptBeg
				globals::var().mlog.OP_moveEnd(&globals::var().newText);
			}

			if (isResizing)
			{
				isResizing = false;
				globals::var().mlog.OP_sizeEnd(&globals::var().newText);
			}
		}
		else
		{
			//stop resizing
			currentCursorMode = 0;

			//disable the moving flag
			if (isMoving)
			{
				isMoving = false;
				//record the new position of ptBeg
				globals::var().mlog.OP_moveEnd(globals::var().selectedObjectPtr);
			}

			if (isResizing)
			{
				isResizing = false;
				globals::var().mlog.OP_sizeEnd(globals::var().selectedObjectPtr);
			}
		}
		InvalidateRect(param.hWnd_, NULL, FALSE);
		mouseHasDown = false;
	}
	return 0;
}

LRESULT WM_KeyDownEvent(Parameter& param)
{
	if (param.wParam_)
	{
		//filter the allowed keys: a~z, 0~9, space/arrors/home/end/backspace/enter
		if ((param.wParam_ >= 65 && param.wParam_ <= 90) ||
			(param.wParam_ >= 48 && param.wParam_ <= 57) ||
			(param.wParam_ >= 0x60 && param.wParam_ <= 0x69) ||
			(param.wParam_ == 0x20) ||
			param.wParam_ == 0x0D ||
			param.wParam_ == 0x08 ||
			param.wParam_ == VK_HOME ||
			param.wParam_ == VK_END ||
			(param.wParam_ >= VK_LEFT && param.wParam_ <= VK_DOWN))
		{
			//continue;
		}
		else
			return 0;

		bool modified = false;
		if ((globals::var().currentDrawMode == 3 && globals::var().newText.startFinished))
		{
			globals::var().mlog.OP_textStart(&globals::var().newText, -1);
			modified = globals::var().newText.KeyIn(param.wParam_);

			int x, y;  //x and y is current caret position on window
			globals::var().newText.CalculateCaretPosition();
			x = globals::var().newText.caretPos.x - xCurrentScroll;
			y = globals::var().newText.caretPos.y - yCurrentScroll;
			mouseX = globals::var().newText.caretPos.x;
			mouseY = globals::var().newText.caretPos.y;
			AutoScroll(param.hWnd_, x, y + 14, xCurrentScroll, yCurrentScroll, rect);
			InvalidateRect(param.hWnd_, NULL, FALSE);

			if (modified)  //make change to log
				globals::var().mlog.OP_textEnd(&globals::var().newText);
		}
		else if (globals::var().currentDrawMode == 4 && globals::var().selectedObjectPtr != nullptr && globals::var().selectedObjectPtr->objectType == 4)
		{
			TextObj * temp;
			temp = (TextObj*)globals::var().selectedObjectPtr;

			auto it = find(globals::var().DrawObjList.begin(), globals::var().DrawObjList.end(), globals::var().selectedObjectPtr);
			if (it != globals::var().DrawObjList.end())
			{
				int pos = distance(globals::var().DrawObjList.begin(), it);
				globals::var().mlog.OP_textStart(globals::var().selectedObjectPtr, pos);

				modified = temp->KeyIn(param.wParam_);

				int x, y;  //x and y is current caret position on window
				temp->CalculateCaretPosition();
				x = temp->caretPos.x - xCurrentScroll;
				y = temp->caretPos.y - yCurrentScroll;
				mouseX = temp->caretPos.x;
				mouseY = temp->caretPos.y;
				AutoScroll(param.hWnd_, x, y + 14, xCurrentScroll, yCurrentScroll, rect);
				InvalidateRect(param.hWnd_, NULL, FALSE);
			}

			if (modified)  //make change to log
				globals::var().mlog.OP_textEnd(globals::var().selectedObjectPtr);
		}
	}
	return 0;
}

LRESULT WM_PaintEvent(Parameter& param)
{
	PAINTSTRUCT ps;
	RECT clientRec = rect;
	HDC hdc = BeginPaint(param.hWnd_, &ps);  //this will return display device id
	

	GetClientRect(param.hWnd_, &clientRec);
	HDC memoryDC = CreateCompatibleDC(hdc);
	hBmp = CreateCompatibleBitmap(hdc, clientRec.right, clientRec.bottom);  // Create a bitmap big enough for our client rectangle.

																			// Select the bitmap into the off-screen DC.
																			//HBITMAP hbmOld;
	SelectObject(memoryDC, hBmp);

	// Erase the background.
	HBRUSH hbrBkGnd;
	hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	FillRect(memoryDC, &clientRec, hbrBkGnd);
	DeleteObject(hbrBkGnd);

	if (globals::var().modifyState == 1) //add a * at file name
	{
		string title = globals::var().fileName;
		title.append("*");
		SetTitle(title, param.hWnd_);
	}
	else  //remove last * at file name
	{
		SetTitle(globals::var().fileName, param.hWnd_);
	}

	// TODO: 在此加入任何使用 hdc 的繪圖程式碼...
	//---------------------------------------------------------------

	//TextOut(memoryDC, 10 - xCurrentScroll, 30 - yCurrentScroll, TEXT("123456"), 6);
	//TextOutA(memoryDC, 10 - xCurrentScroll, 50 - yCurrentScroll, info, strlen(info));  //TextOutA ???
	//TextOutA(memoryDC, 10 - xCurrentScroll, 70 - yCurrentScroll, s.c_str(), s.length());

	string s2 = "";
	for (auto& it : globals::var().DrawObjList)  //Draw each object in DrawObjList
	{
		it->Paint(memoryDC, xCurrentScroll, yCurrentScroll);
	}

	newLine.Paint(memoryDC, xCurrentScroll, yCurrentScroll);
	newRect.Paint(memoryDC, xCurrentScroll, yCurrentScroll);
	newCircle.Paint(memoryDC, xCurrentScroll, yCurrentScroll);
	globals::var().newText.Paint(memoryDC, xCurrentScroll, yCurrentScroll);

	if (mouseHasDown)
	{
		switch (globals::var().currentDrawMode)
		{
		case 0:
			newLine.PaintMouseOnRect(memoryDC, xCurrentScroll, yCurrentScroll);
			break;
		case 1:
			newRect.PaintMouseOnRect(memoryDC, xCurrentScroll, yCurrentScroll);
			break;
		case 2:
			newCircle.PaintMouseOnRect(memoryDC, xCurrentScroll, yCurrentScroll);
			break;
		default:
			break;
		}		
	}


	//s2 = "mouse on state=";
	if (globals::var().preSelectObject != NULL)
		globals::var().preSelectObject->PaintMouseOnRect(memoryDC, xCurrentScroll, yCurrentScroll);

	if (globals::var().hasSelected)
		globals::var().selectedObjectPtr->PaintSelectedRect(memoryDC, xCurrentScroll, yCurrentScroll);

	if (globals::var().currentDrawMode == 3 && globals::var().newText.startFinished && !globals::var().newText.endFinished)
	{
		// Create a solid black caret. 
		CreateCaret(param.hWnd_, (HBITMAP)NULL, 3, 14);
		SetCaretPos(globals::var().newText.caretPos.x - xCurrentScroll, globals::var().newText.caretPos.y - yCurrentScroll);
		/*s2 = "caretPos=" + to_string(globals::var().newText.caretPos.x) + ", " + to_string(globals::var().newText.caretPos.y);
		TextOutA(memoryDC, 500 - xCurrentScroll, 500 - yCurrentScroll, s2.c_str(), s2.length());*/
	}
	else if (globals::var().hasSelected && globals::var().currentDrawMode == 4 && globals::var().selectedObjectPtr->objectType == 4)
	{
		TextObj* t = (TextObj *)globals::var().selectedObjectPtr;
		CreateCaret(param.hWnd_, (HBITMAP)NULL, 3, 14);
		SetCaretPos(t->caretPos.x - xCurrentScroll, t->caretPos.y - yCurrentScroll);
	}
	else
		DestroyCaret();//HideCaret(param.hWnd_);

	s2 = "xCurrentScroll=" + to_string(xCurrentScroll) + " yCurrentScroll=" + to_string(yCurrentScroll);
	TextOutA(memoryDC, 700 - xCurrentScroll, 640 - yCurrentScroll, s2.c_str(), s2.length());
	s2 = "mousex=" + to_string(mouseX) + " mousey=" + to_string(mouseY);
	TextOutA(memoryDC, 700 - xCurrentScroll, 620 - yCurrentScroll, s2.c_str(), s2.length());
	s2 = "currentDrawMode = " + to_string(globals::var().currentDrawMode);
	TextOutA(memoryDC, 700 - xCurrentScroll, 660 - yCurrentScroll, s2.c_str(), s2.length());
	s2 = "object count: " + to_string(globals::var().DrawObjList.size());
	TextOutA(memoryDC, 700 - xCurrentScroll, 680 - yCurrentScroll, s2.c_str(), s2.length());

	/*for (int i = 0; i < 2000; )
	{
	s2 = " " + to_string(i);
	TextOutA(memoryDC, 0- xCurrentScroll, i - yCurrentScroll, s2.c_str(), s2.length());
	TextOutA(memoryDC, i - xCurrentScroll, 0 - yCurrentScroll, s2.c_str(), s2.length());
	i += 200;
	}*/

	//---------------------------------------------------------------
	// Blt the changes to the screen DC.
	BitBlt(hdc,
		clientRec.left, clientRec.top,
		clientRec.right - clientRec.left, clientRec.bottom - clientRec.top,
		memoryDC,
		0, 0,
		SRCCOPY);

	DeleteDC(memoryDC);  //release a memory DC
	DeleteObject(hBmp);
	EndPaint(param.hWnd_, &ps);
	//ReleaseDC(param.hWnd_, hdc);
	return 0;
}

LRESULT WM_SizeEvent(Parameter& param)
{
	rect.left = 0;
	rect.top = 0;
	rect.right = LOWORD(param.lParam_);
	rect.bottom = HIWORD(param.lParam_);

	int xNewSize;
	int yNewSize;
	xNewSize = LOWORD(param.lParam_);
	yNewSize = HIWORD(param.lParam_);

	if (fBlt)
		fSize = TRUE;

	// The horizontal scrolling range is defined by 
	// (bitmap_width) - (client_width). The current horizontal 
	// scroll value remains within the horizontal scrolling range. 
	xMaxScroll = max(bmp.bmWidth - xNewSize, 0);
	xCurrentScroll = min(xCurrentScroll, xMaxScroll);
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = xMinScroll;
	si.nMax = bmp.bmWidth;
	si.nPage = xNewSize;
	si.nPos = xCurrentScroll;
	SetScrollInfo(param.hWnd_, SB_HORZ, &si, TRUE);

	// The vertical scrolling range is defined by 
	// (bitmap_height) - (client_height). The current vertical 
	// scroll value remains within the vertical scrolling range. 
	if (hScrollResize < 5)
		hScrollResize++;
	if (hScrollResize != 2) //ignore for 2nd time. still don't know why??
	{
		yMaxScroll = max(bmp.bmHeight - yNewSize, 0);
	}
	yCurrentScroll = min(yCurrentScroll, yMaxScroll);
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = yMinScroll;
	si.nMax = bmp.bmHeight;
	si.nPage = yNewSize;
	si.nPos = yCurrentScroll;
	SetScrollInfo(param.hWnd_, SB_VERT, &si, TRUE);

	if (globals::var().myChildWindow)
		SetWindowPos(globals::var().myChildWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	InvalidateRect(param.hWnd_, NULL, false);
	return 0;
}

LRESULT WM_HScrollEvent(Parameter& param)
{
	int xDelta;     // xDelta = new_pos - current_pos  
	int xNewPos;    // new position 
	int yDelta = 0;

	switch (LOWORD(param.wParam_))
	{
		// User clicked the scroll bar shaft left of the scroll box. 
	case SB_PAGEUP:
		xNewPos = xCurrentScroll - 50;
		break;

		// User clicked the scroll bar shaft right of the scroll box. 
	case SB_PAGEDOWN:
		xNewPos = xCurrentScroll + 50;
		break;

		// User clicked the left arrow. 
	case SB_LINEUP:
		xNewPos = xCurrentScroll - 5;
		break;

		// User clicked the right arrow. 
	case SB_LINEDOWN:
		xNewPos = xCurrentScroll + 5;
		break;

		// User dragged the scroll box. 
	case SB_THUMBTRACK:
		xNewPos = HIWORD(param.wParam_);
		break;

	default:
		xNewPos = xCurrentScroll;
	}

	// New position must be between 0 and the screen width. 
	xNewPos = max(0, xNewPos);
	xNewPos = min(xMaxScroll, xNewPos);

	// If the current position does not change, do not scroll.
	if (xNewPos == xCurrentScroll)
		return 0;

	// Set the scroll flag to TRUE. 
	fScroll = TRUE;

	// Determine the amount scrolled (in pixels).
	xDelta = xNewPos - xCurrentScroll;

	// Reset the current scroll position. 
	xCurrentScroll = xNewPos;

	// Scroll the window. (The system repaints most of the 
	// client area when ScrollWindowEx is called; however, it is 
	// necessary to call UpdateWindow in order to repaint the 
	// rectangle of pixels that were invalidated.) 
	if (!globals::var().autoScrolling)
	{
		ScrollWindowEx(param.hWnd_, -xDelta, -yDelta, (CONST RECT *) NULL,
			(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
			SW_INVALIDATE);
		UpdateWindow(param.hWnd_);
	}

	// Reset the scroll bar. 
	si.cbSize = sizeof(si);
	si.fMask = SIF_POS;
	si.nPos = xCurrentScroll;
	SetScrollInfo(param.hWnd_, SB_HORZ, &si, TRUE);
	return 0;
}

LRESULT WM_VScrollEvent(Parameter& param)
{
	int xDelta = 0;
	int yDelta;     // yDelta = new_pos - current_pos 
	int yNewPos;    // new position 

	switch (LOWORD(param.wParam_))
	{
		// User clicked the scroll bar shaft above the scroll box. 
	case SB_PAGEUP:
		yNewPos = yCurrentScroll - 50;
		break;

		// User clicked the scroll bar shaft below the scroll box. 
	case SB_PAGEDOWN:
		yNewPos = yCurrentScroll + 50;
		break;

		// User clicked the top arrow. 
	case SB_LINEUP:
		yNewPos = yCurrentScroll - 5;
		break;

		// User clicked the bottom arrow. 
	case SB_LINEDOWN:
		yNewPos = yCurrentScroll + 5;
		break;

		// User dragged the scroll box. 
	case SB_THUMBTRACK:
		yNewPos = HIWORD(param.wParam_);
		if (yNewPos == 1004)
			yNewPos = yMaxScroll;  //workaround for Y scroll to bottom
		break;

	default:
		yNewPos = yCurrentScroll;
	}

	//if (yNewPos > yCurrentScroll)
	//	yNewPos = yCurrentScroll + 10;
	//else if (yNewPos < yCurrentScroll)
	//	yNewPos = yCurrentScroll - 10;

	// New position must be between 0 and the screen height. 
	yNewPos = max(0, yNewPos);
	yNewPos = min(yMaxScroll, yNewPos);

	// If the current position does not change, do not scroll.
	if (yNewPos == yCurrentScroll)
		return 0;

	// Set the scroll flag to TRUE. 
	fScroll = TRUE;

	// Determine the amount scrolled (in pixels). 
	yDelta = yNewPos - yCurrentScroll;

	// Reset the current scroll position. 
	yCurrentScroll = yNewPos;

	// Scroll the window. (The system repaints most of the 
	// client area when ScrollWindowEx is called; however, it is 
	// necessary to call UpdateWindow in order to repaint the 
	// rectangle of pixels that were invalidated.) 
	if (!globals::var().autoScrolling)
	{
		ScrollWindowEx(param.hWnd_, -xDelta, -yDelta, (CONST RECT *) NULL,  // <-Don't use this! Just call InvalidateRect
			NULL, (HRGN)NULL, (PRECT)NULL,       //   and redraw the all area.
			SW_INVALIDATE);
		UpdateWindow(param.hWnd_);
	}

	// Reset the scroll bar. 
	si.cbSize = sizeof(si);
	si.fMask = SIF_POS;
	si.nPos = yCurrentScroll;
	SetScrollInfo(param.hWnd_, SB_VERT, &si, TRUE);
	return 0;
}

LRESULT WM_CloseEvent(Parameter& param)
{
	if (globals::var().modifyState == 1)
	{
		if (DisplayConfirmNewFileMessageBox(globals::var().fileName) == IDYES)
		{
			PushCurrentNewText(globals::var().newText);
			if (globals::var().lastFilePath.size() < 1)
			{
				SaveToFile(globals::var().DrawObjList, globals::var().fileName);
				SetTitle(globals::var().fileName, param.hWnd_);
				globals::var().modifyState = 2;
			}
			else
			{
				SaveToLastFilePath(globals::var().DrawObjList);
				globals::var().modifyState = 2;
			}
		}
	}
	DestroyWindow(param.hWnd_);
	return 0;
}

LRESULT WM_DestroyEvent(Parameter& param)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT WM_SetCursorEvent(Parameter& param)
{
	return 0;
}

LRESULT WM_GetMinMaxInfo(Parameter & param)  //set the min size of window
{
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)param.lParam_;
	lpMMI->ptMinTrackSize.x = 77;
	lpMMI->ptMinTrackSize.y = 415;
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

POINT MovePastedObj()
{
	int xdir, ydir;
	if (xPasteDir == 0 )
	{
		xdir = -30;
		//ydir = -30;
	}
	else
	{
		xdir = 30;
		//ydir = -30;
	}
	
	if (yPasteDir == 0)
	{
		//xdir = 30;
		ydir = -30;
	}
	else
	{
		//xdir = -30;
		ydir = 30;
	}

	POINT newBeg = globals::var().pastebinObjectPtr->ptBeg;
	POINT newEnd = globals::var().pastebinObjectPtr->ptEnd;
	newBeg.x += xdir;
	newBeg.y += ydir;
	newEnd.x += xdir;
	newEnd.y += ydir;

	if (newBeg.x < 0)
	{
		int delta = 0 - newBeg.x;
		newBeg.x = 0;
		newEnd.x += delta;
		xPasteDir = 1;
	}
	if (newBeg.y < 0)
	{
		int delta = 0 - newBeg.y;
		newBeg.y = 0;
		newEnd.y += delta;
		yPasteDir = 1;
	}

	if (newBeg.x > 1990)
	{
		int delta = 1990 - newBeg.x;
		newBeg.x = 1990;
		newEnd.x += delta;
		xPasteDir = 0;
	}
	if (newBeg.y > 1989)
	{
		int delta = 1990 - newBeg.y;
		newBeg.y = 1990;
		newEnd.y += delta;
		yPasteDir = 0;
	}
	//dealing with newEnd
	if (newEnd.x < 0)
	{
		int delta = 0 - newEnd.x;
		newEnd.x = 0;
		newBeg.x += delta;
		xPasteDir = 1;
	}
	if (newEnd.y < 0)
	{
		int delta = 0 - newEnd.y;
		newEnd.y = 0;
		newBeg.y += delta;
		yPasteDir = 1;
	}

	if (newEnd.x > 1990)
	{
		int delta = 1990 - newEnd.x;
		newEnd.x = 1990;
		newBeg.x += delta;
		xPasteDir = 0;
	}
	if (newEnd.y > 1989)
	{
		int delta = 1990 - newEnd.y;
		newEnd.y = 1990;
		newBeg.y += delta;
		yPasteDir = 0;
	}

	globals::var().pastebinObjectPtr->ptBeg = newBeg;
	globals::var().pastebinObjectPtr->ptEnd = newEnd;

	int xfocus, yfocus;
	if (xPasteDir == 0)
		xfocus = newBeg.x < newEnd.x ? newBeg.x : newEnd.x;
	else
		xfocus = newBeg.x > newEnd.x ? newBeg.x : newEnd.x;

	if(yPasteDir == 0)
		yfocus = newBeg.y < newEnd.y ? newBeg.y : newEnd.y;
	else
		yfocus = newBeg.y > newEnd.y ? newBeg.y : newEnd.y;

	POINT r;
	r.x = xfocus;
	r.y = yfocus;
	return r;
}

void PushCurrentNewText(TextObj& T)
{
	if (T.text.size() > 0 && T.text.back().size() > 0)
	{
		T.endFinished = true;
		globals::var().DrawObjList.push_back(new TextObj(T));
		globals::var().mlog.OP_add(&T);
	}
	T.clean();
	globals::var().selectedObjectPtr = nullptr;
	globals::var().hasSelected = false;
}

void Undo()
{
	HMENU hMenu2 = GetSubMenu(hMenu, 1);
	EnableMenuItem(hMenu2, ID_Redo, MF_BYCOMMAND | MF_ENABLED);
	EnableMenuItem(hMenu2, ID_Undo, MF_BYCOMMAND | MF_GRAYED);
	globals::var().mlog.Undo();
}

void Redo()
{
	//swap the two lists
	HMENU hMenu2 = GetSubMenu(hMenu, 1);
	EnableMenuItem(hMenu2, ID_Redo, MF_BYCOMMAND | MF_GRAYED);
	EnableMenuItem(hMenu2, ID_Undo, MF_BYCOMMAND | MF_ENABLED);
	globals::var().mlog.Redo();
}

void UpdateNewText(vector<string> vs, POINT in)
{
	globals::var().newText.text = vs;
	//int x, y;  //x and y is current caret position on window
	globals::var().newText.inputPos = in;
	globals::var().newText.CalculateCaretPosition();
	//x = globals::var().newText.caretPos.x - xCurrentScroll;
	//y = globals::var().newText.caretPos.y - yCurrentScroll;
	//mouseX = globals::var().newText.caretPos.x;
	//mouseY = globals::var().newText.caretPos.y;
	//AutoScroll(param.hWnd_, x, y + 10, xCurrentScroll, yCurrentScroll, rect);
	//InvalidateRect(param.hWnd_, NULL, FALSE);
}

void ToggleUndoButton()
{
	HMENU hMenu2 = GetSubMenu(hMenu, 1);
	EnableMenuItem(hMenu2, ID_Redo, MF_BYCOMMAND | MF_GRAYED);
	EnableMenuItem(hMenu2, ID_Undo, MF_BYCOMMAND | MF_ENABLED);
	globals::var().modifyState = 1;
}
