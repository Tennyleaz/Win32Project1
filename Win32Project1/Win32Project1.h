#pragma once
#include "resource.h"

// 這個程式碼模組中所包含之函式的向前宣告:
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
void				ChangeColorsSelectionState(int, HMENU);