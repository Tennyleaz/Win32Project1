// Win32Project1.cpp : 定義應用程式的進入點。
//

#include "stdafx.h"
#include "Win32Project1.h"
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
//#include <list>
#include "Windowsx.h"
#include "Windows.h"
#include "DrawObj.h"
#include "Save.h"
#include "Listener.h"
#include "WM_Command.h"
//#include "WinProcVars.h"
#include "globals.h"

using namespace std;

/*#define MAX_LOADSTRING 100

// 全域變數: 
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱
WCHAR szChildClass[]=L"123";
HWND hWndFather, myChildWindow;
HWND myButton[5];
int currentDrawMode = 0;     //0=line, 1=rect, 2=circle, 3=text, 4=select 
int modifyState = 0;  //0=new file, 1=modified but not saved, 2=saved or opened
string fileName = "Untitiled";
auto lastStateObject = NULL;  //上一步用的物件
auto recoverObject = NULL;  //下一步用的
auto pastebinObject = NULL;  //剪貼簿用的
DrawObj* selectedObject = NULL;  //選取工具選中的*/

/*// 這個程式碼模組中所包含之函式的向前宣告: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void				AutoScroll(HWND, int, int, int, int, RECT);
RECT				getLocalCoordinates(HWND hWnd);
void				CleanObjects(HWND);
void				SetTitle(string, HWND);
void				PushCurrentNewText(TextObj&);
void				ChangeToolsSelectionState(int, HMENU);
void				ChangeColorsSelectionState(int, HMENU);*/

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此置入程式碼。
	hInst = hInstance;
	Listener::WinProcMsgListener().hInst = hInstance;

	// 初始化全域字串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	//Listener::MyWinProcMessageListener().AddDefaultEvent(DefaultEvnetHandler);
	Listener::WinProcMsgListener().AddEvent(WM_COMMAND, WM_CommandEvent);
	//Listener::MyWinProcMessageListener().AddEvent(WM_LBUTTONUP, ExampleEvnetHandler1);

	// 執行應用程式初始設定: 
	if (!InitInstance(hInstance, SW_MAXIMIZE))
	{
		return FALSE;
	}

	//create my child window
	WNDCLASSEXW childClass;
	childClass.cbSize = sizeof(WNDCLASSEX);
	childClass.style = CS_HREDRAW | CS_VREDRAW;
	childClass.lpfnWndProc = ChildWndProc;
	childClass.cbClsExtra = 0;
	childClass.cbWndExtra = 0;
	childClass.hInstance = hInstance;
	childClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	childClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	childClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	childClass.lpszMenuName = L"Child";
	childClass.lpszClassName = szChildClass;
	childClass.hIconSm = LoadIcon(childClass.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&childClass);

	myChildWindow = CreateWindow(szChildClass, L"工具", WS_CAPTION | WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 77, 320, hWndFather, (HMENU)103, hInst, NULL);

	myButton[0] = CreateWindow(L"BUTTON", L"B1", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 5, 50, 50, myChildWindow, (HMENU)120, hInst, NULL);
	myButton[1] = CreateWindow(L"BUTTON", L"B2", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 60, 50, 50, myChildWindow, (HMENU)121, hInst, NULL);
	myButton[2] = CreateWindow(L"BUTTON", L"B3", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 115, 50, 50, myChildWindow, (HMENU)122, hInst, NULL);
	myButton[3] = CreateWindow(L"BUTTON", L"B4", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 170, 50, 50, myChildWindow, (HMENU)123, hInst, NULL);
	myButton[4] = CreateWindow(L"BUTTON", L"B5", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 225, 50, 50, myChildWindow, (HMENU)124, hInst, NULL);
	
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	MSG msg;

	// 主訊息迴圈: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函式: MyRegisterClass()
//
//  用途: 註冊視窗類別。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函式: InitInstance(HINSTANCE, int)
//
//   用途: 儲存執行個體控制代碼並且建立主視窗
//
//   註解: 
//
//        在這個函式中，我們會將執行個體控制代碼儲存在全域變數中，
//        並且建立和顯示主程式視窗。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

 	hWndFather = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_CLIPCHILDREN |WS_CLIPSIBLINGS,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWndFather)
	{
		return FALSE;
	}

	ShowWindow(hWndFather, nCmdShow);
	UpdateWindow(hWndFather);

	return TRUE;
}

//
//  函式: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  用途:     處理主視窗的訊息。
//
//  WM_COMMAND  - 處理應用程式功能表
//  WM_PAINT    - 繪製主視窗
//  WM_DESTROY  - 顯示結束訊息然後返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/*static int mouseX, mouseY;
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
	static int currentCursorMode;   //0=original 1=左上 2=右下 3=右上 4=左下 5=左 6=右 7=上 8=下 9=四向
	static bool hasSelected;
	static HCURSOR cursors[6];      //0=original 1=左上右下 2=右上左下 3=左右 4=上下 5=四向
	//string debugmessage = "cursorX=";*/

	Listener::WinProcMsgListener().Trig(message, Parameter(hWnd, message, lParam, wParam));

	switch (message)
	{
	case WM_CREATE:
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
		hMenu = GetMenu(hWnd); //LoadMenu(NULL, MAKEINTRESOURCE(IDC_WIN32PROJECT1));
		if (hMenu)
		{			
			//ModifyMenu(hMenu, ID_COMMAND_1, MF_BYCOMMAND | MF_STRING, ID_COMMAND_1, (PCTSTR)(LONG)bmpIcon1);
			HMENU hMenu2 = GetSubMenu(hMenu, 2);   //hMenu2 = 工具
			HMENU hMenu3 = GetSubMenu(hMenu2, 6);  //hMenu3 = 顏色
			
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
			MessageBox(hWnd, 0, TEXT("NO MENU"), MB_OK);

		cursors[0] = LoadCursor(NULL, IDC_ARROW);  // 0 = default mouse
		cursors[1] = LoadCursor(NULL, IDC_SIZENWSE);  //1~2 = 四角的游標 1=左上右下
		cursors[2] = LoadCursor(NULL, IDC_SIZENESW);  //2=右上左下
		cursors[3] = LoadCursor(NULL, IDC_SIZEWE);  //3~4 = 邊上的游標 3=左右
		cursors[4] = LoadCursor(NULL, IDC_SIZENS);  //4=上下
		cursors[5] = LoadCursor(NULL, IDC_SIZEALL);

		// Create a normal DC and a memory DC for the entire 
		// screen. The normal DC provides a snapshot of the 
		// screen contents. The memory DC keeps a copy of this 
		// snapshot in the associated bitmap. 
		hdcScreen = CreateDC(L"DISPLAY", (PCTSTR)NULL,	(PCTSTR)NULL, (CONST DEVMODE *) NULL);
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
	/*case WM_COMMAND:
	{	
		int wmId = LOWORD(wParam);
		// 剖析功能表選取項目: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			if (modifyState == 1)
			{
				if (DisplayConfirmNewFileMessageBox(fileName) == IDYES)
					SaveToFile(DrawObjList, fileName);
			}
			DestroyWindow(hWnd);
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
			CleanObjects(hWnd);
			break;
		case 120:
			SetFocus(hWnd);  //return the focus back to main window
		case ID_LineTool:
			currentDrawMode = 0;
			ChangeToolsSelectionState(currentDrawMode, hMenu);
			PushCurrentNewText(newText);
			hasSelected = false;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 121:
			SetFocus(hWnd);
		case ID_RectTool:
			currentDrawMode = 1;			
			ChangeToolsSelectionState(currentDrawMode, hMenu);
			PushCurrentNewText(newText);
			hasSelected = false;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 122:
			SetFocus(hWnd);
		case ID_CircleTool:
			currentDrawMode = 2;
			ChangeToolsSelectionState(currentDrawMode, hMenu);
			PushCurrentNewText(newText);
			hasSelected = false;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 123:
			SetFocus(hWnd);
		case ID_TextTool:
			currentDrawMode = 3;
			hasSelected = false;
			ChangeToolsSelectionState(currentDrawMode, hMenu);
			break;
		case 124:
			SetFocus(hWnd);
		case ID_SelectTool:
			currentDrawMode = 4;
			PushCurrentNewText(newText);
			ChangeToolsSelectionState(currentDrawMode, hMenu);
			break;
		case ID_BLACK:
			currentColor = 0;			
			newText.color = currentColor;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case ID_GRAY:
			currentColor = 1;
			ChangeColorsSelectionState(currentColor, hMenu);
			newText.color = currentColor;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case ID_RED:
			currentColor = 2;
			ChangeColorsSelectionState(currentColor, hMenu);
			newText.color = currentColor;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case ID_GREEN:
			currentColor = 3;
			ChangeColorsSelectionState(currentColor, hMenu);
			newText.color = currentColor;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case ID_BLU:
			currentColor = 4;
			ChangeColorsSelectionState(currentColor, hMenu);
			newText.color = currentColor;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case ID_CYAN:
			currentColor = 5;
			ChangeColorsSelectionState(currentColor, hMenu);
			newText.color = currentColor;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case ID_YELLOW:
			currentColor = 6;
			ChangeColorsSelectionState(currentColor, hMenu);
			newText.color = currentColor;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case ID_Magenta:
			currentColor = 7;
			ChangeColorsSelectionState(currentColor, hMenu);
			newText.color = currentColor;
			InvalidateRect(hWnd, NULL, FALSE);
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
			SetTitle(fileName, hWnd);
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
			SetTitle(fileName, hWnd);
			newLine.clean();
			newRect.clean();
			newText.clean();
			newCircle.clean();
			selectedObject = NULL;
			currentCursorMode = 0;
			hasSelected = false;
			CleanObjects(hWnd);
			break;
		case ID_OPEN_FILE:
		{			
			//cleanObjects(hWnd);
			ReadFromFile(DrawObjList, fileName);
			newLine.clean();
			newRect.clean();
			newText.clean();
			newCircle.clean();
			SetTitle(fileName, hWnd);
			InvalidateRect(hWnd, NULL, FALSE);
			modifyState = 2;
			break;
		}
		default:
			wsprintf(szBuffer, TEXT("Button ID %d : %d"), wParam, lParam);
			MessageBox(hWnd, szBuffer, TEXT("Pressed"), MB_OK);
			return DefWindowProc(hWnd, message, wParam, lParam);
		}		
		break;
	}	*/
	case WM_MOUSEMOVE:
	{
		
		if (currentDrawMode!=4 && mouseHasDown)  //scroll the scrollbars when mouse out of range
		{
			mouseX = GET_X_LPARAM(lParam);
			mouseY = GET_Y_LPARAM(lParam);
			AutoScroll(hWnd, mouseX, mouseY, xCurrentScroll, yCurrentScroll, rect);
		}

		if (currentDrawMode == 0)
		{
			if (!newLine.endFinished && newLine.startFinished)
			{
				newLine.ptEnd.x = GET_X_LPARAM(lParam) + xCurrentScroll;
				newLine.ptEnd.y = GET_Y_LPARAM(lParam) + yCurrentScroll;
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}
		else if (currentDrawMode == 1)
		{
			if (!newRect.endFinished && newRect.startFinished)
			{
				newRect.ptEnd.x = GET_X_LPARAM(lParam) + xCurrentScroll;
				newRect.ptEnd.y = GET_Y_LPARAM(lParam) + yCurrentScroll;
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}
		else if (currentDrawMode == 2)
		{
			if (!newCircle.endFinished && newCircle.startFinished)
			{
				newCircle.ptEnd.x = GET_X_LPARAM(lParam) + xCurrentScroll;
				newCircle.ptEnd.y = GET_Y_LPARAM(lParam) + yCurrentScroll;
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}
		else if (currentDrawMode == 3)
		{	/*do nothing on text*/	}
		else 
		{
			//if mouse is not down on object, only change the mouse icon
			if (hasSelected && !mouseHasDown)
			{
				//draw a double arrow mouse if mouse is on the 8-points
				mouseX = GET_X_LPARAM(lParam) + xCurrentScroll;
				mouseY = GET_Y_LPARAM(lParam) + yCurrentScroll;
				currentCursorMode =selectedObject->CheckMouseIsOnSizingOpint(mouseX, mouseY);
				//if(currentCursorMode == 0)
					SetCursor(cursors[(currentCursorMode+1)/2]);
				//else if(currentCursorMode == 1 || currentCursorMode == 2)

				//draw a moving arrow if mouse is on the object
				if (currentCursorMode == 0 && selectedObject->CheckObjectCollision(mouseX, mouseY))
				{
					SetCursor(cursors[5]);
					currentCursorMode = 9;
				}
			}
			else if (hasSelected && mouseHasDown)  //if mouse is down on object, then perform move/resize
			{
				mouseX = GET_X_LPARAM(lParam) + xCurrentScroll;
				mouseY = GET_Y_LPARAM(lParam) + yCurrentScroll;
				if (currentCursorMode == 9)  //perform move
				{
					selectedObject->Moving(mouseX, mouseY);
					InvalidateRect(hWnd, NULL, FALSE);
				}
				else if (currentCursorMode > 0 && currentCursorMode < 9)  //perform resize
				{
					selectedObject->Resizing(mouseX, mouseY, currentCursorMode);
					InvalidateRect(hWnd, NULL, FALSE);
				}
			}
			
			//else
				//SetCursor(cursors[0]); //do nothing
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (wParam && !mouseHasDown)  //if mouse L is down
		{
			SetCapture(hWnd);  //capture mouse even outside window
			mouseX = GET_X_LPARAM(lParam) + xCurrentScroll;
			mouseY = GET_Y_LPARAM(lParam) + yCurrentScroll;
			if (currentDrawMode == 0)
			{
				newLine.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
				modifyState = 1;
			}
			else if (currentDrawMode == 1)
			{
				newRect.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
				modifyState = 1;
			}
			else if (currentDrawMode == 2)
			{
				newCircle.makeStart(mouseX, mouseY, currentColor, currentBgColor, currentLineWidth);
				modifyState = 1;
			}
			else if (currentDrawMode == 3)
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
				modifyState = 1;
			}
			else
			{
				//if mouse is on the 8 sizing point, start to resize
				if (currentCursorMode != 0)
				{
					mouseHasDown = true;
					selectedObject->StartToMove(mouseX, mouseY);
					return 0;
				}
				
				//if mouse is on Object, start to drag

				//if no sizing or moving, detect a collision and select an object
				//check mouse & DrawObjList have collision or not (from tail)				
				for (auto it = DrawObjList.crbegin(); it != DrawObjList.crend(); ++it)
				{
					if ((*it)->CheckObjectCollision(mouseX, mouseY))
					{
						selectedObject = (*it);
						hasSelected = true;
						break;
					}
				}

				//draw a selected rectangle
				mouseHasDown = false;
				InvalidateRect(hWnd, NULL, FALSE);
				return 0;
			}
			InvalidateRect(hWnd, NULL, FALSE);
			mouseHasDown = true;			
		}
		return 0;
	}
	case WM_LBUTTONUP:
	{
		if (!wParam)
		{
			ReleaseCapture();  //stop capture mouse
			if (currentDrawMode == 0)
			{
				newLine.makeEnd(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), xCurrentScroll, yCurrentScroll);
				if(newLine.ptBeg.x != newLine.ptEnd.x || newLine.ptBeg.y != newLine.ptEnd.y)
					DrawObjList.push_back(new LineObj(newLine));
				newLine.startFinished = false;
				//DrawObjList.push_back(move(make_unique<LineObj>(newline)));
			}
			else if (currentDrawMode == 1)
			{
				newRect.makeEnd(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), xCurrentScroll, yCurrentScroll);
				if (newRect.ptBeg.x != newRect.ptEnd.x || newRect.ptBeg.y != newRect.ptEnd.y)
					DrawObjList.push_back(new RectangularObj(newRect));
				newRect.startFinished = false;
				//DrawObjList.push_back(move(make_unique<RectangularObj>(newRect)));
			}
			else if (currentDrawMode == 2)
			{
				newCircle.makeEnd(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), xCurrentScroll, yCurrentScroll);
				if(newCircle.ptBeg.x != newCircle.ptEnd.x || newCircle.ptBeg.y != newCircle.ptEnd.y)
					DrawObjList.push_back(new CircleObj(newCircle));
				newCircle.startFinished = false;
				//DrawObjList.push_back(move(make_unique<CircleObj>(newCircle)));
			}
			else if (currentDrawMode == 3)
			{	/*do nothing for newText*/	}
			else
			{
				//stop resizing
				currentCursorMode = 0;
			}
			InvalidateRect(hWnd, NULL, FALSE);
			mouseHasDown = false;
		}
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (wParam)
		{
			if (currentDrawMode == 3 && newText.startFinished)
			{			
				if ((wParam >= 65 && wParam <= 90) || (wParam >= 48 && wParam <= 57) || (wParam >= 0x60 && wParam <= 0x69) || (wParam == 0x20))
				{
					if (newText.ptBeg.x + newText.text.back().size() * 8 > 1987) //if x > 2000 add new line and add new char
					{
						int newy = newText.ptBeg.y + (newText.text.size() + 1) * 13;
						if (newy < 1989)  //if y < 2000 add new line
						{
							newText.addNewLine();
						}
						else
							break;  //do nothing
					}					
					newText.addChar(wParam);
				}
				else if (wParam == 0x0D)  //enter
				{
					if (newText.ptBeg.y + (newText.text.size() + 1) * 13 < 1988)
					{
						newText.addNewLine();  //insert a "" string to back
					}
				}
				else if (wParam == 0x08)  //backspace <-
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
				AutoScroll(hWnd, x, y+10, xCurrentScroll, yCurrentScroll, rect);
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		RECT clientRec = rect;
		HDC hdc = BeginPaint(hWnd, &ps);  //this will return display device id
		HBRUSH hbrBkGnd;

		GetClientRect(hWnd, &clientRec);
		HDC memoryDC = CreateCompatibleDC(hdc);
		hBmp = CreateCompatibleBitmap(hdc, clientRec.right, clientRec.bottom);  // Create a bitmap big enough for our client rectangle.

		// Select the bitmap into the off-screen DC.
		//HBITMAP hbmOld;
		SelectObject(memoryDC, hBmp);

		// Erase the background.
		hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
		FillRect(memoryDC, &clientRec, hbrBkGnd);
		DeleteObject(hbrBkGnd);

		if (modifyState == 1) //add a * at file name
		{
			string title = fileName;
			title.append("*");
			SetTitle(title, hWnd);
		}
		else  //remove last * at file name
		{
			SetTitle(fileName, hWnd);
		}

		// TODO: 在此加入任何使用 hdc 的繪圖程式碼...
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

		if (currentDrawMode == 3 && newText.startFinished && !newText.endFinished)
		{
			// Create a solid black caret. 
			CreateCaret(hWnd, (HBITMAP)NULL, 3, 14);
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
			DestroyCaret();//HideCaret(hWnd);		

		for (auto& it : DrawObjList)  //Draw each object in DrawObjList
		{
			it->Paint(memoryDC, xCurrentScroll, yCurrentScroll);
		}

		if (hasSelected)
			selectedObject->PaintSelectedRect(memoryDC, xCurrentScroll, yCurrentScroll);

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
		EndPaint(hWnd, &ps);
		//ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_SIZE:
	{
		/*static int cx, cy;
		cx = LOWORD(lParam);
		cx = HIWORD(lParam);*/
		//rect.left = 24 * cxChar;
		//rect.top = 2 * cyChar;
		rect.left = 0;
		rect.top = 0;
		rect.right = LOWORD(lParam) - 10;
		rect.bottom = HIWORD(lParam) - 10;

		int xNewSize;
		int yNewSize;
		xNewSize = LOWORD(lParam);
		yNewSize = HIWORD(lParam);

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
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

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
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		if(myChildWindow)
			SetWindowPos(myChildWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		InvalidateRect(hWnd, NULL, false);
		return 0;
	}
	break;
	case WM_HSCROLL:
	{
		int xDelta;     // xDelta = new_pos - current_pos  
		int xNewPos;    // new position 
		int yDelta = 0;

		switch (LOWORD(wParam))
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
			xNewPos = HIWORD(wParam);
			break;

		default:
			xNewPos = xCurrentScroll;
		}

		// New position must be between 0 and the screen width. 
		xNewPos = max(0, xNewPos);
		xNewPos = min(xMaxScroll, xNewPos);

		// If the current position does not change, do not scroll.
		if (xNewPos == xCurrentScroll)
			break;

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
		ScrollWindowEx(hWnd, -xDelta, -yDelta, (CONST RECT *) NULL,
			(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
			SW_INVALIDATE);
		UpdateWindow(hWnd);

		// Reset the scroll bar. 
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		si.nPos = xCurrentScroll;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

		break;
	}
	case  WM_VSCROLL:
	{
		int xDelta = 0;
		int yDelta;     // yDelta = new_pos - current_pos 
		int yNewPos;    // new position 

		switch (LOWORD(wParam))
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
			yNewPos = HIWORD(wParam);
			break;

		default:
			yNewPos = yCurrentScroll;
		}

		// New position must be between 0 and the screen height. 
		yNewPos = max(0, yNewPos);
		yNewPos = min(yMaxScroll, yNewPos);

		// If the current position does not change, do not scroll.
		if (yNewPos == yCurrentScroll)
			break;

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
		ScrollWindowEx(hWnd, -xDelta, -yDelta, (CONST RECT *) NULL,
			(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
			SW_INVALIDATE);
		UpdateWindow(hWnd);

		// Reset the scroll bar. 
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		si.nPos = yCurrentScroll;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



/*// [關於] 方塊的訊息處理常式。
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
}*/

//the callback class for child window
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int wmId;
	static HBITMAP bmpIcon1, bmpIcon2, bmpIcon3, bmpIcon4, bmpIcon5;   //a bitmap icon for button

	switch (message)
	{
	case WM_CREATE:
		bmpIcon1 = (HBITMAP)LoadImage(NULL, L"line.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon2 = (HBITMAP)LoadImage(NULL, L"rect.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon3 = (HBITMAP)LoadImage(NULL, L"circle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon4 = (HBITMAP)LoadImage(NULL, L"text.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon5 = (HBITMAP)LoadImage(NULL, L"select.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		break;
	case WM_SIZE:
		//InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		// 剖析功能表選取項目: 
		SendMessage(hWndFather, WM_COMMAND, wmId, 0);	
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//image for child window buttons
		SendMessage(myButton[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon1);
		SendMessage(myButton[1], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon2);
		SendMessage(myButton[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon3);
		SendMessage(myButton[3], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon4);
		SendMessage(myButton[4], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon5);
		SendMessage(myButton[currentDrawMode], BM_SETSTATE, BN_PUSHED, 0);

		if (!bmpIcon1)
			MessageBox(hWnd, 0, TEXT("NO IMAGE"), MB_OK);

		EndPaint(hWnd,&ps);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_DESTROY:
		DeleteObject(bmpIcon1);
		DeleteObject(bmpIcon2);
		DeleteObject(bmpIcon3);
		DeleteObject(bmpIcon4);
		DeleteObject(bmpIcon5);
		PostQuitMessage(0);
		break;
	case WM_WINDOWPOSCHANGING:  //passed when dragging
	{
		WINDOWPOS *pos = (WINDOWPOS *)lParam;
		RECT rectFather;
		GetWindowRect(hWndFather, &rectFather);

		if (pos->x < 0)
			pos->x = 0;

		if (pos->y < 0)
			pos->y = 0;

		if (pos->y > rectFather.bottom - rectFather.top - 397)
			pos->y = rectFather.bottom - rectFather.top - 397;

		if (pos->x > rectFather.right - rectFather.left - 110)
			pos->x = rectFather.right - rectFather.left - 110;
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return NULL;
}

//scroll the window if x and y focus are out of border
void AutoScroll(HWND hwnd, int Xfocus, int Yfocus, int xCurrentScroll, int yCurrentScroll, RECT windowRect)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	if (Xfocus > (rect.right-rect.left-29) && xCurrentScroll < 2000)
	{
		WPARAM wParam;
		if (currentDrawMode == 3)
			wParam = MAKEWPARAM(SB_THUMBTRACK, xCurrentScroll + (Xfocus - windowRect.right)+1);
		else
			wParam = MAKEWPARAM(SB_THUMBTRACK, xCurrentScroll + (Xfocus - windowRect.right));
		SendMessage(hwnd, WM_HSCROLL, wParam, NULL);
	}
	else if (xCurrentScroll > 0 && Xfocus <= 0)
	{		
		WPARAM wParam;
		if (currentDrawMode == 3)
			wParam = MAKEWPARAM(SB_THUMBTRACK, (xCurrentScroll + Xfocus-8) < 0 ? 0 : xCurrentScroll + Xfocus - 8);  //留個空位給新輸入文字
		else
			wParam = MAKEWPARAM(SB_THUMBTRACK, (xCurrentScroll + Xfocus) <= 5 ? 0 : xCurrentScroll + Xfocus - 5);
		SendMessage(hwnd, WM_HSCROLL, wParam, NULL);
	}

	if (Yfocus > windowRect.bottom && yCurrentScroll < 2000)
	{
		WPARAM wParam = MAKEWPARAM(SB_THUMBTRACK, yCurrentScroll + (Yfocus - windowRect.bottom));
		SendMessage(hwnd, WM_VSCROLL, wParam, NULL);
	}
	else if (yCurrentScroll > 0 && Yfocus < 0)
	{
		WPARAM wParam;
		if (currentDrawMode == 3)
			wParam = MAKEWPARAM(SB_THUMBTRACK, (yCurrentScroll + Yfocus -10 ) < 0 ? 0 : yCurrentScroll + Yfocus -10);
		else
			wParam = MAKEWPARAM(SB_THUMBTRACK, (yCurrentScroll + Yfocus) < 0 ? 0 : yCurrentScroll + Yfocus);
		SendMessage(hwnd, WM_VSCROLL, wParam, NULL);
	}
}

RECT getLocalCoordinates(HWND hWnd)
{
	RECT Rect;
	GetWindowRect(hWnd, &Rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&Rect, 2);
	return Rect;
}

void CleanObjects(HWND hWnd)
{
	for (auto& it : DrawObjList)  //delete each pointer. 
		delete(it);
	DrawObjList.clear();  //clear() does not delete memory! WTF! (or use smart pointers) (line 170)
	InvalidateRect(hWnd, NULL, TRUE);
}

void SetTitle(string name, HWND hWnd)
{
	wstring stemp = wstring(name.begin(), name.end());
	LPCWSTR sw = stemp.c_str();
	SetWindowText(hWnd, sw);
}

void PushCurrentNewText(TextObj& newText)
{
	if (newText.text.size() > 0 && newText.text.back().size() > 0)
	{
		newText.endFinished = true;
		DrawObjList.push_back(new TextObj(newText));
	}
	newText.clean();
}

void ChangeToolsSelectionState(int position, HMENU hMenu)
{
	HMENU hMenu2 = GetSubMenu(hMenu, 2);   //hMenu2 = 工具
	for (int i = 0; i < 5; i++)
	{
		if (i == position)
		{
			SendMessage(myButton[i], BM_SETSTATE, BN_PUSHED, 0);
			CheckMenuItem(hMenu2, i, MF_CHECKED | MF_BYPOSITION);
		}
		else
		{
			SendMessage(myButton[i], BM_SETSTATE, 0, 0);
			CheckMenuItem(hMenu2, i, MF_UNCHECKED | MF_BYPOSITION);
		}
	}
}

void ChangeColorsSelectionState(int position, HMENU hMenu)
{
	HMENU hMenu2 = GetSubMenu(hMenu, 2);   //hMenu2 = 工具
	HMENU hMenu3 = GetSubMenu(hMenu2, 6);  //hMenu3 = 顏色
	for (int i = 0; i < 8; i++)
	{
		if (i == position)
		{
			CheckMenuItem(hMenu3, i, MF_CHECKED | MF_BYPOSITION);
		}
		else
		{
			CheckMenuItem(hMenu3, i, MF_UNCHECKED | MF_BYPOSITION);
		}
	}
}