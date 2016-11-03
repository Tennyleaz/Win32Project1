#pragma once
#include "Resource.h"
#include <string>
#include "DrawObj.h"

// �o�ӵ{���X�Ҳդ��ҥ]�t���禡���V�e�ŧi:
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
void				ChangeBGSelectionState(int, HMENU);
void				ChangeLineSelectionState(int, HMENU);