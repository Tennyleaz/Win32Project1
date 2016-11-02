#include "stdafx.h"
#include "Windowsx.h "
#include "WM_Command.h"
#include "Resource.h"
#include "globals.h"
#include "Save.h"
#include "Win32Project1.h"

static int mouseX, mouseY;
static LineObj  newLine;
static RectangularObj newRect;
static TextObj newText;
static CircleObj newCircle;
static bool mouseHasDown = false;

static RECT rect;
static TCHAR szBuffer[50];
static int cxChar, cyChar;
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
static HBITMAP bmpIcon1, bmpIcon2, bmpIcon3, bmpIcon4, bmpIcon5, bmpIcon6, bmpIcon7, bmpIcon8;   //a bitmap icon for button
static HBITMAP checkedIcon;
static HMENU hMenu=NULL;        //try to get the system menu
static HBITMAP hBmp;          //bitmap for memory DC

static int currentColor;        //0=black, 0~7 kinds of color
static int currentBgColor;      //0=transparent, 0~7 kinds of color
static int currentLineWidth;    //width 1~5
static int currentCursorMode;   //0=original 1=���W 2=�k�U 3=�k�W 4=���U 5=�� 6=�k 7=�W 8=�U 9=�|�V
static bool hasSelected;
static HCURSOR cursors[6];      //0=original 1=���W�k�U 2=�k�W���U 3=���k 4=�W�U 5=�|�V
//string debugmessage = "cursorX=";

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
	// ��R�\���������: 
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(globals::var().hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), param.hWnd_, About);
		break;
	case IDM_EXIT:
		if (globals::var().modifyState == 1)
		{
			if (DisplayConfirmNewFileMessageBox(globals::var().fileName) == IDYES)
				SaveToFile(globals::var().DrawObjList, globals::var().fileName);
		}
		DestroyWindow(param.hWnd_);
		break;
	case ID_COMMAND_1:  //�M��
		if (globals::var().modifyState == 1)
		{
			if (DisplayConfirmClearMessageBox(globals::var().fileName) == IDYES)
				SaveToFile(globals::var().DrawObjList, globals::var().fileName);
		}
		globals::var().modifyState = 1;
		newLine.clean();
		newRect.clean();
		newText.clean();
		newCircle.clean();
		globals::var().selectedObject = NULL;
		currentCursorMode = 0;
		hasSelected = false;
		CleanObjects(param.hWnd_);
		break;
	case 120:
		SetFocus(param.hWnd_);  //return the focus back to main window
	case ID_LineTool:
		globals::var().currentDrawMode = 0;
		ChangeToolsSelectionState(globals::var().currentDrawMode, hMenu);
		PushCurrentNewText(newText);
		hasSelected = false;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case 121:
		SetFocus(param.hWnd_);
	case ID_RectTool:
		globals::var().currentDrawMode = 1;
		ChangeToolsSelectionState(globals::var().currentDrawMode, hMenu);
		PushCurrentNewText(newText);
		hasSelected = false;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case 122:
		SetFocus(param.hWnd_);
	case ID_CircleTool:
		globals::var().currentDrawMode = 2;
		ChangeToolsSelectionState(globals::var().currentDrawMode, hMenu);
		PushCurrentNewText(newText);
		hasSelected = false;
		InvalidateRect(param.hWnd_, NULL, FALSE);
		break;
	case 123:
		SetFocus(param.hWnd_);
	case ID_TextTool:
		globals::var().currentDrawMode = 3;
		hasSelected = false;
		ChangeToolsSelectionState(globals::var().currentDrawMode, hMenu);
		break;
	case 124:
		SetFocus(param.hWnd_);
	case ID_SelectTool:
		globals::var().currentDrawMode = 4;
		PushCurrentNewText(newText);
		ChangeToolsSelectionState(globals::var().currentDrawMode, hMenu);
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
		if (globals::var().modifyState == 0 || lastFilePath == NULL || wcslen(lastFilePath) < 1)
			goto SAVE_AS_NEW_FILE;
		PushCurrentNewText(newText);
		SaveToLastFilePath(globals::var().DrawObjList);
		globals::var().modifyState = 2;
		break;
	case ID_SAVE_AS:
	{
	SAVE_AS_NEW_FILE:
		PushCurrentNewText(newText);
		SaveToFile(globals::var().DrawObjList, globals::var().fileName);
		SetTitle(globals::var().fileName, param.hWnd_);
		globals::var().modifyState = 2;
		break;
	}
	case ID_NEW_FILE:
		if (globals::var().modifyState == 1)
		{
			if (DisplayConfirmNewFileMessageBox(globals::var().fileName) == IDYES)
				SaveToFile(globals::var().DrawObjList, globals::var().fileName);
		}
		globals::var().modifyState = 0;
		globals::var().fileName = "Untitled";
		lastFilePath = NULL;
		SetTitle(globals::var().fileName, param.hWnd_);
		newLine.clean();
		newRect.clean();
		newText.clean();
		newCircle.clean();
		globals::var().selectedObject = NULL;
		currentCursorMode = 0;
		hasSelected = false;
		CleanObjects(param.hWnd_);
		break;
	case ID_OPEN_FILE:
	{
		//cleanObjects(param.hWnd_);
		ReadFromFile(globals::var().DrawObjList, globals::var().fileName);
		newLine.clean();
		newRect.clean();
		newText.clean();
		newCircle.clean();
		SetTitle(globals::var().fileName, param.hWnd_);
		InvalidateRect(param.hWnd_, NULL, FALSE);
		globals::var().modifyState = 2;
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
	hasSelected = false;
	cxChar = LOWORD(GetDialogBaseUnits());
	cyChar = HIWORD(GetDialogBaseUnits());

	//image for child window buttons
	bmpIcon1 = (HBITMAP)LoadImage(NULL, L"black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon2 = (HBITMAP)LoadImage(NULL, L"grey.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon3 = (HBITMAP)LoadImage(NULL, L"red.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon4 = (HBITMAP)LoadImage(NULL, L"green.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon5 = (HBITMAP)LoadImage(NULL, L"blu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon6 = (HBITMAP)LoadImage(NULL, L"cyan.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon7 = (HBITMAP)LoadImage(NULL, L"yellow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bmpIcon8 = (HBITMAP)LoadImage(NULL, L"magenta.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	checkedIcon = (HBITMAP)LoadImage(NULL, L"checkedIcon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//add image to menu
	hMenu = GetMenu(param.hWnd_); //LoadMenu(NULL, MAKEINTRESOURCE(IDC_WIN32PROJECT1));
	if (hMenu)
	{
		//ModifyMenu(hMenu, ID_COMMAND_1, MF_BYCOMMAND | MF_STRING, ID_COMMAND_1, (PCTSTR)(LONG)bmpIcon1);
		HMENU hMenu2 = GetSubMenu(hMenu, 2);   //hMenu2 = �u��
		HMENU hMenu3 = GetSubMenu(hMenu2, 6);  //hMenu3 = �C��

		ModifyMenu(hMenu3, 0, MF_BYPOSITION | MF_BITMAP, ID_BLACK, (LPCTSTR)bmpIcon1);
		ModifyMenu(hMenu3, 1, MF_BYPOSITION | MF_BITMAP, ID_GRAY, (LPCTSTR)bmpIcon2);
		ModifyMenu(hMenu3, 2, MF_BYPOSITION | MF_BITMAP, ID_RED, (LPCTSTR)bmpIcon3);
		ModifyMenu(hMenu3, 3, MF_BYPOSITION | MF_BITMAP, ID_GREEN, (LPCTSTR)bmpIcon4);
		ModifyMenu(hMenu3, 4, MF_BYPOSITION | MF_BITMAP, ID_BLU, (LPCTSTR)bmpIcon5);
		ModifyMenu(hMenu3, 5, MF_BYPOSITION | MF_BITMAP, ID_CYAN, (LPCTSTR)bmpIcon6);
		ModifyMenu(hMenu3, 6, MF_BYPOSITION | MF_BITMAP, ID_YELLOW, (LPCTSTR)bmpIcon7);
		ModifyMenu(hMenu3, 7, MF_BYPOSITION | MF_BITMAP, ID_Magenta, (LPCTSTR)bmpIcon8);
		CheckMenuItem(hMenu3, ID_BLACK, MF_CHECKED);
		SetMenuItemBitmaps(hMenu3, 0, MF_BYPOSITION, NULL, checkedIcon);
		SetMenuItemBitmaps(hMenu3, 1, MF_BYPOSITION, NULL, checkedIcon);
		SetMenuItemBitmaps(hMenu3, 2, MF_BYPOSITION, NULL, checkedIcon);
		SetMenuItemBitmaps(hMenu3, 3, MF_BYPOSITION, NULL, checkedIcon);
		SetMenuItemBitmaps(hMenu3, 4, MF_BYPOSITION, NULL, checkedIcon);
		SetMenuItemBitmaps(hMenu3, 5, MF_BYPOSITION, NULL, checkedIcon);
		SetMenuItemBitmaps(hMenu3, 6, MF_BYPOSITION, NULL, checkedIcon);
		SetMenuItemBitmaps(hMenu3, 7, MF_BYPOSITION, NULL, checkedIcon);

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
	cursors[1] = LoadCursor(NULL, IDC_SIZENWSE);  //1~2 = �|������� 1=���W�k�U
	cursors[2] = LoadCursor(NULL, IDC_SIZENESW);  //2=�k�W���U
	cursors[3] = LoadCursor(NULL, IDC_SIZEWE);  //3~4 = ��W����� 3=���k
	cursors[4] = LoadCursor(NULL, IDC_SIZENS);  //4=�W�U
	cursors[5] = LoadCursor(NULL, IDC_SIZEALL);

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
	DeleteDC(hdcScreen);
	return 0;
}

LRESULT WM_MouseMoveEvent(Parameter& param)
{
	if (globals::var().currentDrawMode != 4 && mouseHasDown)  //scroll the scrollbars when mouse out of range
	{
		mouseX = GET_X_LPARAM(param.lParam_);
		mouseY = GET_Y_LPARAM(param.lParam_);
		AutoScroll(param.hWnd_, mouseX, mouseY, xCurrentScroll, yCurrentScroll, rect);
	}

	if (globals::var().currentDrawMode == 0)
	{
		if (!newLine.endFinished && newLine.startFinished)
		{
			newLine.ptEnd.x = GET_X_LPARAM(param.lParam_) + xCurrentScroll;
			newLine.ptEnd.y = GET_Y_LPARAM(param.lParam_) + yCurrentScroll;
			InvalidateRect(param.hWnd_, NULL, FALSE);
		}
	}
	else if (globals::var().currentDrawMode == 1)
	{
		if (!newRect.endFinished && newRect.startFinished)
		{
			newRect.ptEnd.x = GET_X_LPARAM(param.lParam_) + xCurrentScroll;
			newRect.ptEnd.y = GET_Y_LPARAM(param.lParam_) + yCurrentScroll;
			InvalidateRect(param.hWnd_, NULL, FALSE);
		}
	}
	else if (globals::var().currentDrawMode == 2)
	{
		if (!newCircle.endFinished && newCircle.startFinished)
		{
			newCircle.ptEnd.x = GET_X_LPARAM(param.lParam_) + xCurrentScroll;
			newCircle.ptEnd.y = GET_Y_LPARAM(param.lParam_) + yCurrentScroll;
			InvalidateRect(param.hWnd_, NULL, FALSE);
		}
	}
	else if (globals::var().currentDrawMode == 3)
	{	/*do nothing on text*/
	}
	else
	{
		//if mouse is not down on object, only change the mouse icon
		if (hasSelected && !mouseHasDown)
		{
			//draw a double arrow mouse if mouse is on the 8-points
			mouseX = GET_X_LPARAM(param.lParam_) + xCurrentScroll;
			mouseY = GET_Y_LPARAM(param.lParam_) + yCurrentScroll;
			currentCursorMode = globals::var().selectedObject->CheckMouseIsOnSizingOpint(mouseX, mouseY);
			//if(currentCursorMode == 0)
			SetCursor(cursors[(currentCursorMode + 1) / 2]);
			//else if(currentCursorMode == 1 || currentCursorMode == 2)

			//draw a moving arrow if mouse is on the object
			if (currentCursorMode == 0 && globals::var().selectedObject->CheckObjectCollision(mouseX, mouseY))
			{
				SetCursor(cursors[5]);
				currentCursorMode = 9;
			}
		}
		else if (hasSelected && mouseHasDown)  //if mouse is down on object, then perform move/resize
		{
			mouseX = GET_X_LPARAM(param.lParam_) + xCurrentScroll;
			mouseY = GET_Y_LPARAM(param.lParam_) + yCurrentScroll;
			if (currentCursorMode == 9)  //perform move
			{
				globals::var().selectedObject->Moving(mouseX, mouseY);
				InvalidateRect(param.hWnd_, NULL, FALSE);
			}
			else if (currentCursorMode > 0 && currentCursorMode < 9)  //perform resize
			{
				globals::var().selectedObject->Resizing(mouseX, mouseY, currentCursorMode);
				InvalidateRect(param.hWnd_, NULL, FALSE);
			}
		}

		//else
		//SetCursor(cursors[0]); //do nothing
	}
	//break;
	return 0;
}

LRESULT WM_LButtonDownEvent(Parameter& param)
{
	if (param.wParam_ && !mouseHasDown)  //if mouse L is down
	{
		SetCapture(param.hWnd_);  //capture mouse even outside window
		mouseX = GET_X_LPARAM(param.lParam_) + xCurrentScroll;
		mouseY = GET_Y_LPARAM(param.lParam_) + yCurrentScroll;
		if (globals::var().currentDrawMode == 0)
		{
			newLine.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
			globals::var().modifyState = 1;
		}
		else if (globals::var().currentDrawMode == 1)
		{
			newRect.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
			globals::var().modifyState = 1;
		}
		else if (globals::var().currentDrawMode == 2)
		{
			newCircle.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
			globals::var().modifyState = 1;
		}
		else if (globals::var().currentDrawMode == 3)
		{
			if (!newText.startFinished) //click to a new text position without previous start
			{
				newText.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
			}
			else if (newText.startFinished && !newText.endFinished)  //push old text to drawObj list
			{
				PushCurrentNewText(newText);
				newText.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
			}
			if (newText.ptBeg.y > 1987)
				newText.ptBeg.y = 1987;
			globals::var().modifyState = 1;
		}
		else
		{
			//if mouse is on the 8 sizing point, start to resize
			if (currentCursorMode != 0)
			{
				mouseHasDown = true;
				globals::var().selectedObject->StartToMove(mouseX, mouseY);
				return 0;
			}

			//if mouse is on Object, start to drag

			//if no sizing or moving, detect a collision and select an object
			//check mouse & DrawObjList have collision or not (from tail)				
			for (auto it = globals::var().DrawObjList.crbegin(); it != globals::var().DrawObjList.crend(); ++it)
			{
				if ((*it)->CheckObjectCollision(mouseX, mouseY))
				{
					globals::var().selectedObject = (*it);
					hasSelected = true;
					break;
				}
			}

			//draw a selected rectangle
			mouseHasDown = false;
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
		if (globals::var().currentDrawMode == 0)
		{
			newLine.makeEnd(GET_X_LPARAM(param.lParam_), GET_Y_LPARAM(param.lParam_), xCurrentScroll, yCurrentScroll);
			if (newLine.ptBeg.x != newLine.ptEnd.x || newLine.ptBeg.y != newLine.ptEnd.y)
				globals::var().DrawObjList.push_back(new LineObj(newLine));
			newLine.startFinished = false;
			//DrawObjList.push_back(move(make_unique<LineObj>(newline)));
		}
		else if (globals::var().currentDrawMode == 1)
		{
			newRect.makeEnd(GET_X_LPARAM(param.lParam_), GET_Y_LPARAM(param.lParam_), xCurrentScroll, yCurrentScroll);
			if (newRect.ptBeg.x != newRect.ptEnd.x || newRect.ptBeg.y != newRect.ptEnd.y)
				globals::var().DrawObjList.push_back(new RectangularObj(newRect));
			newRect.startFinished = false;
			//DrawObjList.push_back(move(make_unique<RectangularObj>(newRect)));
		}
		else if (globals::var().currentDrawMode == 2)
		{
			newCircle.makeEnd(GET_X_LPARAM(param.lParam_), GET_Y_LPARAM(param.lParam_), xCurrentScroll, yCurrentScroll);
			if (newCircle.ptBeg.x != newCircle.ptEnd.x || newCircle.ptBeg.y != newCircle.ptEnd.y)
				globals::var().DrawObjList.push_back(new CircleObj(newCircle));
			newCircle.startFinished = false;
			//DrawObjList.push_back(move(make_unique<CircleObj>(newCircle)));
		}
		else if (globals::var().currentDrawMode == 3)
		{	/*do nothing for newText*/
		}
		else
		{
			//stop resizing
			currentCursorMode = 0;
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
		if (globals::var().currentDrawMode == 3 && newText.startFinished)
		{
			if ((param.wParam_ >= 65 && param.wParam_ <= 90) || (param.wParam_ >= 48 && param.wParam_ <= 57) || (param.wParam_ >= 0x60 && param.wParam_ <= 0x69) || (param.wParam_ == 0x20))
			{
				if (newText.ptBeg.x + newText.text.back().size() * 8 > 1987) //if x > 2000 add new line and add new char
				{
					int newy = newText.ptBeg.y + (newText.text.size() + 1) * 13;
					if (newy < 1989)  //if y < 2000 add new line
					{
						newText.addNewLine();
					}
					else
						return 0;  //do nothing
				}
				newText.addChar(param.wParam_);
			}
			else if (param.wParam_ == 0x0D)  //enter
			{
				if (newText.ptBeg.y + (newText.text.size() + 1) * 13 < 1988)
				{
					newText.addNewLine();  //insert a "" string to back
				}
			}
			else if (param.wParam_ == 0x08)  //backspace <-
			{
				newText.backspace();
			}

			int x, y;  //x and y is current caret position on window
			if (newText.text.size() > 0)
			{
				x = newText.ptBeg.x + newText.text.back().size() * 8 - xCurrentScroll;
				y = newText.ptBeg.y + (newText.text.size() - 1) * 13 - yCurrentScroll;
			}
			else
			{
				x = newText.ptBeg.x - xCurrentScroll;
				y = newText.ptBeg.y - yCurrentScroll;
			}
			mouseX = x;
			mouseY = y;
			AutoScroll(param.hWnd_, x, y + 10, xCurrentScroll, yCurrentScroll, rect);
			InvalidateRect(param.hWnd_, NULL, FALSE);
		}
	}
	return 0;
}

LRESULT WM_PaintEvent(Parameter& param)
{
	PAINTSTRUCT ps;
	RECT clientRec = rect;
	HDC hdc = BeginPaint(param.hWnd_, &ps);  //this will return display device id
	HBRUSH hbrBkGnd;

	GetClientRect(param.hWnd_, &clientRec);
	HDC memoryDC = CreateCompatibleDC(hdc);
	hBmp = CreateCompatibleBitmap(hdc, clientRec.right, clientRec.bottom);  // Create a bitmap big enough for our client rectangle.

																			// Select the bitmap into the off-screen DC.
																			//HBITMAP hbmOld;
	SelectObject(memoryDC, hBmp);

	// Erase the background.
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

	// TODO: �b���[�J����ϥ� hdc ��ø�ϵ{���X...
	//---------------------------------------------------------------

	//TextOut(memoryDC, 10 - xCurrentScroll, 30 - yCurrentScroll, TEXT("123456"), 6);
	//TextOutA(memoryDC, 10 - xCurrentScroll, 50 - yCurrentScroll, info, strlen(info));  //TextOutA ???
	//TextOutA(memoryDC, 10 - xCurrentScroll, 70 - yCurrentScroll, s.c_str(), s.length());

	string s2 = "";

	newLine.Paint(memoryDC, xCurrentScroll, yCurrentScroll);
	//SelectObject(memoryDC, GetStockObject(NULL_BRUSH)); //to draw a empty rectangle
	newRect.Paint(memoryDC, xCurrentScroll, yCurrentScroll);
	newCircle.Paint(memoryDC, xCurrentScroll, yCurrentScroll);
	newText.Paint(memoryDC, xCurrentScroll, yCurrentScroll);

	if (globals::var().currentDrawMode == 3 && newText.startFinished && !newText.endFinished)
	{
		// Create a solid black caret. 
		CreateCaret(param.hWnd_, (HBITMAP)NULL, 3, 14);
		int x, y;
		if (newText.text.size() > 0)
		{
			x = newText.ptBeg.x + newText.text.back().size() * 8 - xCurrentScroll;
			y = newText.ptBeg.y + (newText.text.size() - 1) * 13 - yCurrentScroll;
		}
		else
		{
			x = newText.ptBeg.x - xCurrentScroll;
			y = newText.ptBeg.y - yCurrentScroll;
		}
		// Adjust the caret position, in client coordinates. 
		SetCaretPos(x, y);
	}
	else
		DestroyCaret();//HideCaret(param.hWnd_);		

	for (auto& it : globals::var().DrawObjList)  //Draw each object in DrawObjList
	{
		it->Paint(memoryDC, xCurrentScroll, yCurrentScroll);
	}

	if (hasSelected)
		globals::var().selectedObject->PaintSelectedRect(memoryDC, xCurrentScroll, yCurrentScroll);

	s2 = "xCurrentScroll=" + to_string(xCurrentScroll) + " yCurrentScroll=" + to_string(yCurrentScroll);
	TextOutA(memoryDC, 700 - xCurrentScroll, 640 - yCurrentScroll, s2.c_str(), s2.length());
	s2 = "mousex=" + to_string(mouseX) + " mousey=" + to_string(mouseY);
	TextOutA(memoryDC, 700 - xCurrentScroll, 620 - yCurrentScroll, s2.c_str(), s2.length());

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

	// If scrolling has occurred, use the following call to 
	// BitBlt to paint the invalid rectangle. 
	// 
	// The coordinates of this rectangle are specified in the 
	// RECT structure to which prect points. 
	// 
	// Note that it is necessary to increment the seventh 
	// argument (prect->left) by xCurrentScroll and the 
	// eighth argument (prect->top) by yCurrentScroll in 
	// order to map the correct pixels from the source bitmap. 
	//if (fScroll)
	//{
	//	prect = &ps.rcPaint;
	//
	//	BitBlt(hdc,
	//		prect->left, prect->top,
	//		(prect->right - prect->left),
	//		(prect->bottom - prect->top),
	//		memoryDC,
	//		prect->left + xCurrentScroll,
	//		prect->top + yCurrentScroll,
	//		SRCCOPY);
	//
	//	fScroll = FALSE;
	//}

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
	rect.right = LOWORD(param.lParam_) - 10;
	rect.bottom = HIWORD(param.lParam_) - 10;

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
	yMaxScroll = max(bmp.bmHeight - yNewSize, 0);
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
	ScrollWindowEx(param.hWnd_, -xDelta, -yDelta, (CONST RECT *) NULL,
		(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
		SW_INVALIDATE);
	UpdateWindow(param.hWnd_);

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
		break;

	default:
		yNewPos = yCurrentScroll;
	}

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
	ScrollWindowEx(param.hWnd_, -xDelta, -yDelta, (CONST RECT *) NULL,
		(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
		SW_INVALIDATE);
	UpdateWindow(param.hWnd_);

	// Reset the scroll bar. 
	si.cbSize = sizeof(si);
	si.fMask = SIF_POS;
	si.nPos = yCurrentScroll;
	SetScrollInfo(param.hWnd_, SB_VERT, &si, TRUE);
	return 0;
}

LRESULT WM_DestroyEvent(Parameter& param)
{
	PostQuitMessage(0);
	return 0;
}

// [����] ������T���B�z�`���C
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