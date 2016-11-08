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
//#include "Save.h"
#include "Listener.h"
#include "WM_Command.h"
//#include "WinProcVars.h"
#include "globals.h"

using namespace std;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此置入程式碼。	
	globals::var().hInst = hInstance;
	Listener::WinProcMsgListener().hInst = hInstance;

	// 初始化全域字串
	LoadStringW(hInstance, IDS_APP_TITLE, globals::var().szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32PROJECT1, globals::var().szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	//Listener::WinProcMsgListener().AddDefaultEvent(DefaultEvnetHandler);
	Listener::WinProcMsgListener().AddEvent(WM_COMMAND, WM_CommandEvent);
	Listener::WinProcMsgListener().AddEvent(WM_CREATE, WM_CreateEvent);
	Listener::WinProcMsgListener().AddEvent(WM_MOUSEMOVE, WM_MouseMoveEvent);
	Listener::WinProcMsgListener().AddEvent(WM_LBUTTONDOWN, WM_LButtonDownEvent);
	Listener::WinProcMsgListener().AddEvent(WM_LBUTTONUP, WM_LButtonUpEvent);
	Listener::WinProcMsgListener().AddEvent(WM_KEYDOWN, WM_KeyDownEvent);
	Listener::WinProcMsgListener().AddEvent(WM_PAINT, WM_PaintEvent);
	Listener::WinProcMsgListener().AddEvent(WM_SIZE, WM_SizeEvent);
	Listener::WinProcMsgListener().AddEvent(WM_HSCROLL, WM_HScrollEvent);
	Listener::WinProcMsgListener().AddEvent(WM_VSCROLL, WM_VScrollEvent);
	Listener::WinProcMsgListener().AddEvent(WM_DESTROY, WM_DestroyEvent);

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
	childClass.lpszClassName = globals::var().szChildClass;
	childClass.hIconSm = LoadIcon(childClass.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&childClass);

	globals::var().myChildWindow = CreateWindow(globals::var().szChildClass, L"工具", WS_CAPTION | WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 77, 320, globals::var().hWndFather, (HMENU)103, globals::var().hInst, NULL);

	globals::var().myButton[0] = CreateWindow(L"BUTTON", L"B1", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 5, 50, 50, globals::var().myChildWindow, (HMENU)120, globals::var().hInst, NULL);
	globals::var().myButton[1] = CreateWindow(L"BUTTON", L"B2", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 60, 50, 50, globals::var().myChildWindow, (HMENU)121, globals::var().hInst, NULL);
	globals::var().myButton[2] = CreateWindow(L"BUTTON", L"B3", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 115, 50, 50, globals::var().myChildWindow, (HMENU)122, globals::var().hInst, NULL);
	globals::var().myButton[3] = CreateWindow(L"BUTTON", L"B4", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 170, 50, 50, globals::var().myChildWindow, (HMENU)123, globals::var().hInst, NULL);
	globals::var().myButton[4] = CreateWindow(L"BUTTON", L"B5", WS_VISIBLE | WS_CHILD | BS_BITMAP, 5, 225, 50, 50, globals::var().myChildWindow, (HMENU)124, globals::var().hInst, NULL);
	
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
	wcex.lpszClassName = globals::var().szWindowClass;
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
	globals::var().hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

 	globals::var().hWndFather = CreateWindowW(globals::var().szWindowClass, globals::var().szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_CLIPCHILDREN |WS_CLIPSIBLINGS,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!globals::var().hWndFather)
	{
		return FALSE;
	}

	ShowWindow(globals::var().hWndFather, nCmdShow);
	UpdateWindow(globals::var().hWndFather);

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
	return Listener::WinProcMsgListener().Trig(message, Parameter(hWnd, message, wParam, lParam));
}


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
		SendMessage(globals::var().hWndFather, WM_COMMAND, wmId, 0);	
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//image for child window buttons
		SendMessage(globals::var().myButton[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon1);
		SendMessage(globals::var().myButton[1], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon2);
		SendMessage(globals::var().myButton[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon3);
		SendMessage(globals::var().myButton[3], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon4);
		SendMessage(globals::var().myButton[4], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmpIcon5);
		SendMessage(globals::var().myButton[globals::var().currentDrawMode], BM_SETSTATE, BN_PUSHED, 0);

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
		GetWindowRect(globals::var().hWndFather, &rectFather);

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

//scroll the window if x and y focus are out of border. X, Y focus is mouse position on screen
void AutoScroll(HWND hwnd, int Xfocus, int Yfocus, int xCurrentScroll, int yCurrentScroll, RECT windowRect)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	if (Xfocus > (rect.right-rect.left-29) && xCurrentScroll < 2000)
	{
		WPARAM wParam;
		if (globals::var().currentDrawMode == 3)
			wParam = MAKEWPARAM(SB_THUMBTRACK, xCurrentScroll + (Xfocus - windowRect.right)+1);
		else
			wParam = MAKEWPARAM(SB_THUMBTRACK, xCurrentScroll + (Xfocus - windowRect.right));
		SendMessage(hwnd, WM_HSCROLL, wParam, NULL);
	}
	else if (xCurrentScroll > 0 && Xfocus <= 0)
	{		
		WPARAM wParam;
		if (globals::var().currentDrawMode == 3)
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
		if (globals::var().currentDrawMode == 3)
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
	for (auto& it : globals::var().DrawObjList)  //delete each pointer. 
		delete(it);
	globals::var().DrawObjList.clear();  //clear() does not delete memory! WTF! (or use smart pointers) (line 170)
	InvalidateRect(hWnd, NULL, TRUE);
}

void SetTitle(string name, HWND hWnd)
{
	wstring stemp = wstring(name.begin(), name.end());
	LPCWSTR sw = stemp.c_str();
	SetWindowText(hWnd, sw);
}

//void PushCurrentNewText(TextObj& newText)
//{
//	if (newText.text.size() > 0 && newText.text.back().size() > 0)
//	{
//		newText.endFinished = true;
//		globals::var().DrawObjList.push_back(new TextObj(newText));
//	}
//	newText.clean();
//	globals::var().selectedObjectPtr = nullptr;
//}

void ChangeToolsSelectionState(int position, HMENU hMenu)
{
	if (globals::var().selectedObjectPtr != nullptr && position!=4)
		globals::var().selectedObjectPtr = nullptr;
	HMENU hMenu2 = GetSubMenu(hMenu, 2);   //hMenu2 = 工具
	for (int i = 0; i < 5; i++)
	{
		if (i == position)
		{
			SendMessage(globals::var().myButton[i], BM_SETSTATE, BN_PUSHED, 0);
			CheckMenuItem(hMenu2, i, MF_CHECKED | MF_BYPOSITION);
		}
		else
		{
			SendMessage(globals::var().myButton[i], BM_SETSTATE, 0, 0);
			CheckMenuItem(hMenu2, i, MF_UNCHECKED | MF_BYPOSITION);
		}
	}
}

void ChangeColorsSelectionState(int position, HMENU hMenu)
{
	if (globals::var().selectedObjectPtr != nullptr)
	{
		auto it = find(globals::var().DrawObjList.begin(), globals::var().DrawObjList.end(), globals::var().selectedObjectPtr);
		if (it != globals::var().DrawObjList.end())
		{
			int pos = distance(globals::var().DrawObjList.begin(), it);
			globals::var().mlog.OP_modifyStart(globals::var().selectedObjectPtr, pos);
			globals::var().selectedObjectPtr->color = position;
			globals::var().modifyState = 1;
			globals::var().mlog.OP_modifyEnd(globals::var().selectedObjectPtr);
		}
	}
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

void ChangeBGSelectionState(int position, HMENU hMenu)
{
	if (globals::var().selectedObjectPtr != nullptr)
	{
		auto it = find(globals::var().DrawObjList.begin(), globals::var().DrawObjList.end(), globals::var().selectedObjectPtr);
		if (it != globals::var().DrawObjList.end())
		{
			int pos = distance(globals::var().DrawObjList.begin(), it);
			globals::var().mlog.OP_modifyStart(globals::var().selectedObjectPtr, pos);
			globals::var().selectedObjectPtr->backgroundColor = position;
			globals::var().modifyState = 1;
			globals::var().mlog.OP_modifyEnd(globals::var().selectedObjectPtr);
		}
	}
	HMENU hMenu2 = GetSubMenu(hMenu, 2);   //hMenu2 = 工具
	HMENU hMenu5 = GetSubMenu(hMenu2, 8);  //hMenu5 = 底色
	for (int i = 0; i < 8; i++)
	{
		if (i == position)
		{
			CheckMenuItem(hMenu5, i, MF_CHECKED | MF_BYPOSITION);
		}
		else
		{
			CheckMenuItem(hMenu5, i, MF_UNCHECKED | MF_BYPOSITION);
		}
	}
}

void ChangeLineSelectionState(int position, HMENU hMenu)
{
	if (globals::var().selectedObjectPtr != nullptr)
	{
		auto it = find(globals::var().DrawObjList.begin(), globals::var().DrawObjList.end(), globals::var().selectedObjectPtr);
		if (it != globals::var().DrawObjList.end())
		{
			int pos = distance(globals::var().DrawObjList.begin(), it);
			globals::var().mlog.OP_modifyStart(globals::var().selectedObjectPtr, pos);
			globals::var().selectedObjectPtr->lineWidth = position;
			globals::var().mlog.OP_modifyEnd(globals::var().selectedObjectPtr);
		}
	}
	HMENU hMenu2 = GetSubMenu(hMenu, 2);   //hMenu2 = 工具
	HMENU hMenu4 = GetSubMenu(hMenu2, 7);  //hMenu4 = 線寬
	for (int i = 0; i < 5; i++)
	{
		if (i == position-1)
		{
			CheckMenuItem(hMenu4, i, MF_CHECKED | MF_BYPOSITION);
		}
		else
		{
			CheckMenuItem(hMenu4, i, MF_UNCHECKED | MF_BYPOSITION);
		}
	}
}
