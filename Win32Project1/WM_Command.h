#pragma once
#include "stdafx.h"
#include "Listener.h"
#include "DrawObj.h"

LRESULT DefaultEvnetHandler(Parameter& param);

LRESULT WM_CommandEvent(Parameter& param);
LRESULT WM_CreateEvent(Parameter& param);
LRESULT WM_MouseMoveEvent(Parameter& param);
LRESULT WM_LButtonDownEvent(Parameter& param);
LRESULT WM_LButtonUpEvent(Parameter& param);
LRESULT WM_KeyDownEvent(Parameter& param);
LRESULT WM_PaintEvent(Parameter& param);
LRESULT WM_SizeEvent(Parameter& param);
LRESULT WM_HScrollEvent(Parameter& param);
LRESULT WM_VScrollEvent(Parameter& param);
LRESULT WM_DestroyEvent(Parameter& param);

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

POINT MovePastedObj();
void PushCurrentNewText(TextObj&);

void Redo();
//void Checkpoint();
void Undo();
void UpdateNewText(vector<string>, POINT);

void ToggleUndoButton();