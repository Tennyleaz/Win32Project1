// Win32Project1.cpp : 定義應用程式的進入點。
//

#include "stdafx.h"
#include "Win32Project1.h"
#include <string>
#include "Windowsx.h"
#include "Windows.h"
//#include <list>
#include <vector>
#include <memory>
#include <algorithm>
#include "DrawObj.h"

using namespace std;

#define MAX_LOADSTRING 100

// 全域變數: 
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱
WCHAR szChildClass[]=L"123";
HWND hWndFather;
HWND myButton[4];
int currentDrawMode = 0;     //0=line, 1=rect, 2=circle, 3=text

// 這個程式碼模組中所包含之函式的向前宣告: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void				AutoScroll(HWND, int, int, int, int, RECT);

struct
{
	int iStyle;
	TCHAR * szText;
}
button[] =
{
	BS_PUSHBUTTON, TEXT("PushButton"),
	BS_DEFPUSHBUTTON, TEXT("DefPushButton"),
	BS_CHECKBOX, TEXT("CheckBox"),
	BS_CHECKBOX, TEXT("CheckBox")
};

#define NUM (sizeof button / sizeof button[0])

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此置入程式碼。
	hInst = hInstance;

	// 初始化全域字串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 執行應用程式初始設定: 
	if (!InitInstance(hInstance, nCmdShow))
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

	HWND myChildWindow = CreateWindow(szChildClass, L"工具", WS_CAPTION | WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 30, 10, 77, 265, hWndFather, (HMENU)103, hInst, NULL);

	myButton[0] = CreateWindow(L"BUTTON", L"B1", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 5, 50, 50, myChildWindow, (HMENU)120, hInst, NULL);
	myButton[1] = CreateWindow(L"BUTTON", L"B2", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 60, 50, 50, myChildWindow, (HMENU)121, hInst, NULL);
	myButton[2] = CreateWindow(L"BUTTON", L"B3", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 115, 50, 50, myChildWindow, (HMENU)122, hInst, NULL);
	myButton[3] = CreateWindow(L"BUTTON", L"B4", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 170, 50, 50, myChildWindow, (HMENU)123, hInst, NULL);

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

//list<DrawObj*> DrawObjList;             // <-this is garbage don't use it
vector<unique_ptr<DrawObj>> DrawObjList;  // <-use this
/*list<LineObj> LineObjList;
list<RectangularObj> RectObjList;
list<CircleObj> CircleObjList;
list<TextObj> TextObjList;*/
#define IDM_COMMAND_2 17

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
	static int mouseX, mouseY;
	static LineObj  newline;
	static RectangularObj newRect;
	static TextObj newText;
	static CircleObj newCircle;
	static bool mouseHasDown = false;

	static HWND hwndButton[NUM];
	static RECT rect;
	static TCHAR szTop[] = TEXT("message  wparam  lparam"),
		szFormat[] = TEXT("%-16s%04x-%04x    %04x-%04x"),
		szBuffer[50];
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
	static HMENU hMenu=NULL;        //try to get the system menu
	
	static int currentColor = 0;        //0=black, 0~7 kinds of color
	//static HWND myWindow;

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());	

		//myWindow = CreateWindow((LPCTSTR)"BUTTON", (LPCTSTR)"WindowName", WS_CHILD | WS_VISIBLE, 300, 200, 100, 100, hWnd, (HMENU)0xCAC, hInst, &myWindow);
		//myWindow = CreateWindow(L"STATIC", L"MyWindow", WS_VISIBLE | WS_CHILD , 5, 120, 80, 230, hWnd, (HMENU)102, hInst, NULL);
		//CreateWindow((LPCTSTR)"BUTTON", (LPCTSTR)"WindowName", WS_CHILD | WS_VISIBLE, 10, 10, 100, 20, myWindow, (HMENU)103, hInst, NULL);
		//ShowWindow(myWindow, SW_SHOW);
		/*for (i = 0; i < NUM; i++)
			hwndButton[i] = CreateWindow(TEXT("Button"),
				button[i].szText,
				WS_CHILD | WS_VISIBLE | BS_BITMAP,
				cxChar, cyChar*(1 + 3.5 * i) + 120,
				40, 40,
				hWnd, (HMENU)i,
				hInst, NULL);*/

		//image for child window buttons
		bmpIcon1 = (HBITMAP)LoadImage(NULL, L"black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon2 = (HBITMAP)LoadImage(NULL, L"grey.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon3 = (HBITMAP)LoadImage(NULL, L"red.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon4 = (HBITMAP)LoadImage(NULL, L"green.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon5 = (HBITMAP)LoadImage(NULL, L"blu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon6 = (HBITMAP)LoadImage(NULL, L"cyan.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon7 = (HBITMAP)LoadImage(NULL, L"yellow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bmpIcon8 = (HBITMAP)LoadImage(NULL, L"magenta.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		/*if (bmpIcon1)
		{
			SendMessage(hwndButton[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon1);
			SendMessage(hwndButton[1], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon2);
			SendMessage(hwndButton[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon3);
			SendMessage(hwndButton[3], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon4);
		}
		else
			MessageBox(hWnd, 0, TEXT("NO IMAGE"), MB_OK);*/

		//add image to menu
		hMenu = GetMenu(hWnd); //LoadMenu(NULL, MAKEINTRESOURCE(IDC_WIN32PROJECT1));
		if (hMenu)
		{			
			//ModifyMenu(hMenu, ID_COMMAND_1, MF_BYCOMMAND | MF_STRING, ID_COMMAND_1, (PCTSTR)(LONG)bmpIcon1);
			HMENU hMenu2 = GetSubMenu(hMenu, 1);
			HMENU hMenu3 = GetSubMenu(hMenu2, 5);
			//CheckMenuItem(hMenu, ID_BLACK, MF_CHECKED);
			/*ModifyMenu(hMenu3, 0, MF_BYPOSITION | MF_BITMAP, ID_BLACK, (LPCTSTR)bmpIcon1);
			ModifyMenu(hMenu3, 1, MF_BYPOSITION | MF_BITMAP, ID_GRAY, (LPCTSTR)bmpIcon2);
			ModifyMenu(hMenu3, 2, MF_BYPOSITION | MF_BITMAP, ID_RED, (LPCTSTR)bmpIcon3);
			ModifyMenu(hMenu3, 3, MF_BYPOSITION | MF_BITMAP, ID_GREEN, (LPCTSTR)bmpIcon4);
			ModifyMenu(hMenu3, 4, MF_BYPOSITION | MF_BITMAP, ID_BLU, (LPCTSTR)bmpIcon5);
			ModifyMenu(hMenu3, 5, MF_BYPOSITION | MF_BITMAP, ID_CYAN, (LPCTSTR)bmpIcon6);
			ModifyMenu(hMenu3, 6, MF_BYPOSITION | MF_BITMAP, ID_YELLOW, (LPCTSTR)bmpIcon7);
			ModifyMenu(hMenu3, 7, MF_BYPOSITION | MF_BITMAP, ID_Magenta, (LPCTSTR)bmpIcon8);*/
			SetMenuItemBitmaps(hMenu3, 0, MF_BYPOSITION, bmpIcon1, bmpIcon1);
			//bmpIcon1 = (HBITMAP)LoadImage(NULL, L"grey.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			//bmpIcon2 = (HBITMAP)LoadImage(NULL, L"greychecked.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			SetMenuItemBitmaps(hMenu3, 1, MF_BYPOSITION, bmpIcon2, bmpIcon2);
			SetMenuItemBitmaps(hMenu3, 2, MF_BYPOSITION, bmpIcon3, bmpIcon3);
			SetMenuItemBitmaps(hMenu3, 3, MF_BYPOSITION, bmpIcon4, bmpIcon4);
			SetMenuItemBitmaps(hMenu3, 4, MF_BYPOSITION, bmpIcon5, bmpIcon5);
			SetMenuItemBitmaps(hMenu3, 5, MF_BYPOSITION, bmpIcon6, bmpIcon6);
			SetMenuItemBitmaps(hMenu3, 6, MF_BYPOSITION, bmpIcon7, bmpIcon7);
			SetMenuItemBitmaps(hMenu3, 7, MF_BYPOSITION, bmpIcon8, bmpIcon8);
			/*AppendMenu(hMenu3, MF_BITMAP, 111, (PCTSTR)(LONG)bmpIcon1);
			CheckMenuItem(hMenu3, 111, MF_CHECKED);*/
		}
		else
			MessageBox(hWnd, 0, TEXT("NO MENU"), MB_OK);

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
	case WM_COMMAND:
	{
		/*ScrollWindow(hWnd, 0, -cyChar, &rect, &rect);
		hdc = GetDC(hWnd);
		TextOut(hdc, 24 * cxChar, cyChar*(rect.bottom / cyChar - 1), szBuffer,
			wsprintf(szBuffer, szFormat, message == WM_DRAWITEM ? TEXT("WM_DRAWITEM") : TEXT("WM_COMMAND"),
				HIWORD(wParam), LOWORD(wParam),
				HIWORD(lParam), LOWORD(lParam)));
		ReleaseDC(hWnd, hdc);
		ValidateRect(hWnd, &rect);*/		

		int wmId = LOWORD(wParam);
		// 剖析功能表選取項目: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_COMMAND_1:  //清除
			DrawObjList.clear();			
			newline.clean();
			newRect.clean();
			newText.clean();
			newCircle.clean();
			InvalidateRect(hWnd, NULL, TRUE);
			//MessageBox(hWnd, szBuffer, TEXT("Pressed"), MB_OK);
			break;
		case 120:
			SetFocus(hWnd);  //return the focus back to main window
		case ID_LineTool:
			currentDrawMode = 0;
			CheckMenuItem(hMenu, ID_LineTool, MF_CHECKED);  //check the line tool
			CheckMenuItem(hMenu, ID_RectTool, MF_UNCHECKED);  //uncheck others
			CheckMenuItem(hMenu, ID_CircleTool, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_TextTool, MF_UNCHECKED);
			//bmpIcon1 = (HBITMAP)LoadImage(NULL, L"lineSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			SendMessage(myButton[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"lineSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[1], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"rect.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"circle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[3], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"text.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			break;
		case 121:
			SetFocus(hWnd);
		case ID_RectTool:
			currentDrawMode = 1;
			CheckMenuItem(hMenu, ID_LineTool, MF_UNCHECKED);  
			CheckMenuItem(hMenu, ID_RectTool, MF_CHECKED);
			CheckMenuItem(hMenu, ID_CircleTool, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_TextTool, MF_UNCHECKED);
			SendMessage(myButton[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"line.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[1], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"rectSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"circle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[3], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"text.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			break;
		case 122:
			SetFocus(hWnd);
		case ID_CircleTool:
			currentDrawMode = 2;
			CheckMenuItem(hMenu, ID_LineTool, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_RectTool, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_CircleTool, MF_CHECKED);
			CheckMenuItem(hMenu, ID_TextTool, MF_UNCHECKED);
			SendMessage(myButton[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"line.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[1], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"rect.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"circleSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[3], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"text.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			break;
		case 123:
			SetFocus(hWnd);
		case ID_TextTool:
			currentDrawMode = 3;
			CheckMenuItem(hMenu, ID_LineTool, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_RectTool, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_CircleTool, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_TextTool, MF_CHECKED);
			SendMessage(myButton[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"line.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[1], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"rect.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"circle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(myButton[3], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadImage(NULL, L"textSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			break;
		case ID_BLACK:
			currentColor = 0;
			break;
		case ID_GRAY:
			currentColor = 1;
			break;
		case ID_RED:
			currentColor = 2;
			break;
		case ID_GREEN:
			currentColor = 3;
			break;
		case ID_BLU:
			currentColor = 4;
			break;
		case ID_CYAN:
			currentColor = 5;
			break;
		case ID_YELLOW:
			currentColor = 6;
			break;
		case ID_Magenta:
			currentColor = 7;
			break;
		default:
			wsprintf(szBuffer, TEXT("Button ID %d : %d"), wParam, lParam);
			MessageBox(hWnd, szBuffer, TEXT("Pressed"), MB_OK);
			return DefWindowProc(hWnd, message, wParam, lParam);
		}		
		break;
	}	
	case WM_MOUSEMOVE:
	{
		
		if (mouseHasDown)  //scroll the scrollbars when mouse out of range
		{
			mouseX = GET_X_LPARAM(lParam);
			mouseY = GET_Y_LPARAM(lParam);
			AutoScroll(hWnd, mouseX, mouseY, xCurrentScroll, yCurrentScroll, rect);
		}

		if (currentDrawMode == 0)
		{
			if (!newline.endFinished && newline.startFinished)
			{
				newline.ptEnd.x = GET_X_LPARAM(lParam) + xCurrentScroll;
				newline.ptEnd.y = GET_Y_LPARAM(lParam) + yCurrentScroll;
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
		else
		{

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
				newline.ptBeg.x = mouseX;
				newline.ptBeg.y = mouseY;
				newline.ptEnd.x = mouseX;
				newline.ptEnd.y = mouseY;
				newline.startFinished = true;
				newline.endFinished = false;
				newline.color = currentColor;
			}
			else if (currentDrawMode == 1)
			{
				newRect.ptBeg.x = mouseX;
				newRect.ptBeg.y = mouseY;
				newRect.ptEnd.x = mouseX;
				newRect.ptEnd.y = mouseY;
				newRect.startFinished = true;
				newRect.endFinished = false;
				newRect.color = currentColor;
			}
			else if (currentDrawMode == 2)
			{
				newCircle.ptBeg.x = mouseX;
				newCircle.ptBeg.y = mouseY;
				newCircle.ptEnd.x = mouseX;
				newCircle.ptEnd.y = mouseY;
				newCircle.startFinished = true;
				newCircle.endFinished = false;
				newCircle.color = currentColor;
			}
			else
			{
				if (!newText.startFinished) //a new text position
				{
					newText.ptBeg.x = mouseX;
					newText.ptBeg.y = mouseY;
					newText.ptEnd.x = mouseX;
					newText.ptEnd.y = mouseY;
					newText.startFinished = true;
					newText.endFinished = false;
					newText.color = currentColor;
				}
				else if (newText.startFinished && !newText.endFinished)  //push old text to drawObj list
				{
					if (newText.text.size() > 0)
					{
						newText.endFinished = true;
						DrawObjList.push_back(move(make_unique<TextObj>(newText)));
					}
					newText.clean();
					newText.ptBeg.x = mouseX;
					newText.ptBeg.y = mouseY;
					newText.ptEnd.x = mouseX;
					newText.ptEnd.y = mouseY;
					newText.startFinished = true;
					newText.color = currentColor;
				}
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
				newline.ptEnd.x = GET_X_LPARAM(lParam) + xCurrentScroll;
				newline.ptEnd.y = GET_Y_LPARAM(lParam) + yCurrentScroll;
				newline.endFinished = true;
				DrawObjList.push_back(move(make_unique<LineObj>(newline)));
			}
			else if (currentDrawMode == 1)
			{
				newRect.ptEnd.x = GET_X_LPARAM(lParam) + xCurrentScroll;
				newRect.ptEnd.y = GET_Y_LPARAM(lParam) + yCurrentScroll;
				newRect.endFinished = true;
				//DrawObjList.push_back(new RectangularObj(newRect));
				DrawObjList.push_back(move(make_unique<RectangularObj>(newRect)));
			}
			else if (currentDrawMode == 2)
			{
				newCircle.ptEnd.x = GET_X_LPARAM(lParam) + xCurrentScroll;
				newCircle.ptEnd.y = GET_Y_LPARAM(lParam) + yCurrentScroll;
				newCircle.endFinished = true;
				DrawObjList.push_back(move(make_unique<CircleObj>(newCircle)));
			}
			else
			{
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
				if (wParam >= 65 && wParam <= 90)  //A~Z
				{
					if (newText.text.size() == 0)
						newText.text.push_back("");
					newText.text.back().push_back(wParam+32);
				}
				else if (wParam >= 48 && wParam <= 57) //0~9
				{
					if (newText.text.size() == 0)
						newText.text.push_back("");
					newText.text.back().push_back(wParam);
				}
				else if (wParam == 0x20) //space
				{
					if (newText.text.size() == 0)
						newText.text.push_back("");
					newText.text.back().push_back(' ');
				}
				else if (wParam == 0x0D)  //enter
				{
					if (newText.text.size() == 0)
						newText.text.push_back("");
					newText.text.push_back("");  //insert a "" string to back
				}
				else if (wParam == 0x08)  //backspace <-
				{
					if (newText.text.size() > 0)
					{
						if (newText.text.back().size() > 0)  //string is not empty
						{
							newText.text.back().pop_back();
						}
						else
						{
							newText.text.pop_back();
						}
					}
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
				AutoScroll(hWnd, x, y, xCurrentScroll, yCurrentScroll, rect);
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
		HBITMAP hBmp = CreateCompatibleBitmap(hdc, clientRec.right, clientRec.bottom);  // Create a bitmap big enough for our client rectangle.

		// Select the bitmap into the off-screen DC.
		//HBITMAP hbmOld;
		SelectObject(memoryDC, hBmp);

		// Erase the background.
		hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
		FillRect(memoryDC, &clientRec, hbrBkGnd);
		DeleteObject(hbrBkGnd);

		// TODO: 在此加入任何使用 hdc 的繪圖程式碼...
		//---------------------------------------------------------------

		// create string with the information about mouse position.
		char info[100] = "";
		sprintf(info, "Mouse position: %i  %i ", mouseX, mouseY);

		//print client size
		char Hbuffer[10], Lbuffer[10];
		string s = "client height = ";
		_itoa(clientRec.right, Hbuffer, 10);
		_itoa(clientRec.bottom, Lbuffer, 10);
		s = s + Hbuffer + " length=" + Lbuffer;

		TextOut(memoryDC, 10 - xCurrentScroll, 30 - yCurrentScroll, TEXT("123456"), 6);
		TextOutA(memoryDC, 10 - xCurrentScroll, 50 - yCurrentScroll, info, strlen(info));  //TextOutA ???
		TextOutA(memoryDC, 10 - xCurrentScroll, 70 - yCurrentScroll, s.c_str(), s.length());

		string s2 = "new line start.x= ";
		s2 = s2 + to_string(newline.ptBeg.x) + " start.y=" + to_string(newline.ptBeg.y);
		TextOutA(memoryDC, 10 - xCurrentScroll, 90 - yCurrentScroll, s2.c_str(), s2.length());
		s2 = "new line end.x= ";
		s2 = s2 + to_string(newline.ptEnd.x) + " end.y=" + to_string(newline.ptEnd.y);
		TextOutA(memoryDC, 10 - xCurrentScroll, 110 - yCurrentScroll, s2.c_str(), s2.length());

		newline.Paint(memoryDC, xCurrentScroll, yCurrentScroll);

		//for (auto it = LineObjList.begin(); it != LineObjList.end(); it++)
		//{
		//	it->Paint(memoryDC, xCurrentScroll, yCurrentScroll);
		//}

		SelectObject(memoryDC, GetStockObject(NULL_BRUSH)); //to draw a empty rectangle
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
		

		for (auto& it : DrawObjList)
		{
			it->Paint(memoryDC, xCurrentScroll, yCurrentScroll);
		}

		s2 = "xCurrentScroll=" + to_string(xCurrentScroll) + " yCurrentScroll=" + to_string(yCurrentScroll);
		TextOutA(memoryDC, 700 - xCurrentScroll, 640 - yCurrentScroll, s2.c_str(), s2.length());
		s2 = "mousex=" + to_string(mouseX) + " mousey=" + to_string(mouseY);
		TextOutA(memoryDC, 700 - xCurrentScroll, 620 - yCurrentScroll, s2.c_str(), s2.length());
		//s2 = "newRect.ptrBeg.x=" + to_string(newRect.ptBeg.x) + " newRect.ptBeg.y" + to_string(newRect.ptBeg.y);
		//TextOutA(memoryDC, 700 - xCurrentScroll, 600 - yCurrentScroll, s2.c_str(), s2.length());
		//s2 = "color=" + to_string(newline.color);
		//TextOutA(memoryDC, 700 - xCurrentScroll, 580 - yCurrentScroll, s2.c_str(), s2.length());

		for (int i = 0; i < 2000; )
		{			
			s2 = " " + to_string(i);
			TextOutA(memoryDC, 0- xCurrentScroll, i - yCurrentScroll, s2.c_str(), s2.length());
			TextOutA(memoryDC, i - xCurrentScroll, 0 - yCurrentScroll, s2.c_str(), s2.length());
			i += 200;
		}

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

//the callback class for child window
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int wmId;
	static HBITMAP bmpIcon1, bmpIcon2, bmpIcon3, bmpIcon4;   //a bitmap icon for button

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_SIZE:
		//InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		// 剖析功能表選取項目: 
		switch (wmId)
		{
		case 104:
			SendMessage(hWndFather, WM_COMMAND, 110, 0);
			break;
		case 105:
			SendMessage(hWndFather, WM_COMMAND, 111, 0);
			break;
		default:
			SendMessage(hWndFather, WM_COMMAND, wmId, 0);
		}		
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//image for child window buttons
		if (currentDrawMode == 0)
		{
			bmpIcon1 = (HBITMAP)LoadImage(NULL, L"lineSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon2 = (HBITMAP)LoadImage(NULL, L"rect.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon3 = (HBITMAP)LoadImage(NULL, L"circle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon4 = (HBITMAP)LoadImage(NULL, L"text.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		}
		else if (currentDrawMode == 1)
		{
			bmpIcon1 = (HBITMAP)LoadImage(NULL, L"line.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon2 = (HBITMAP)LoadImage(NULL, L"rectSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon3 = (HBITMAP)LoadImage(NULL, L"circle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon4 = (HBITMAP)LoadImage(NULL, L"text.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		}
		else if (currentDrawMode == 2)
		{
			bmpIcon1 = (HBITMAP)LoadImage(NULL, L"lineSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon2 = (HBITMAP)LoadImage(NULL, L"rect.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon3 = (HBITMAP)LoadImage(NULL, L"circleSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon4 = (HBITMAP)LoadImage(NULL, L"text.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		}
		else
		{
			bmpIcon1 = (HBITMAP)LoadImage(NULL, L"lineSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon2 = (HBITMAP)LoadImage(NULL, L"rect.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon3 = (HBITMAP)LoadImage(NULL, L"circle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			bmpIcon4 = (HBITMAP)LoadImage(NULL, L"textSelected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		}
		if (bmpIcon1)
		{
			SendMessage(myButton[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon1);
			SendMessage(myButton[1], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon2);
			SendMessage(myButton[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon3);
			SendMessage(myButton[3], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon4);
		}
		else
			MessageBox(hWnd, 0, TEXT("NO IMAGE"), MB_OK);
		EndPaint(hWnd,&ps);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return NULL;
}

//scroll the window if x and y focus are out of border
void AutoScroll(HWND hwnd, int Xfocus, int Yfocus, int xCurrentScroll, int yCurrentScroll, RECT windowRect)
{
	if (Xfocus > (windowRect.right) && xCurrentScroll < 2000)
	{
		WPARAM wParam = MAKEWPARAM(SB_THUMBTRACK, xCurrentScroll + (Xfocus - windowRect.right));
		SendMessage(hwnd, WM_HSCROLL, wParam, NULL);
	}
	else if (xCurrentScroll > 0 && Xfocus < 0)
	{		
		WPARAM wParam;
		if (currentDrawMode == 3)
			wParam = MAKEWPARAM(SB_THUMBTRACK, (xCurrentScroll + Xfocus-8) < 0 ? 0 : xCurrentScroll + Xfocus - 8);  //留個空位給新輸入文字
		else
			wParam = MAKEWPARAM(SB_THUMBTRACK, (xCurrentScroll + Xfocus) < 0 ? 0 : xCurrentScroll + Xfocus);
		SendMessage(hwnd, WM_HSCROLL, wParam, NULL);
	}

	if (Yfocus > windowRect.bottom && yCurrentScroll < 2000)
	{
		WPARAM wParam = MAKEWPARAM(SB_THUMBTRACK, yCurrentScroll + (Yfocus - windowRect.bottom));
		SendMessage(hwnd, WM_VSCROLL, wParam, NULL);
	}
	else if (yCurrentScroll > 0 && Yfocus < 0)
	{
		WPARAM wParam = MAKEWPARAM(SB_THUMBTRACK, (yCurrentScroll + Yfocus) < 0 ? 0 : yCurrentScroll + Yfocus);
		SendMessage(hwnd, WM_VSCROLL, wParam, NULL);
	}
}