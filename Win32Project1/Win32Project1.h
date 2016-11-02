#pragma once
#include "stdafx.h"
#include "Resource.h"
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

// 這個程式碼模組中所包含之函式的向前宣告:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void				AutoScroll(HWND, int, int, int, int, RECT);
RECT				getLocalCoordinates(HWND hWnd);
void				CleanObjects(HWND);
void				SetTitle(std::string, HWND);
void				PushCurrentNewText(TextObj&);
void				ChangeToolsSelectionState(int, HMENU);
void				ChangeColorsSelectionState(int, HMENU);