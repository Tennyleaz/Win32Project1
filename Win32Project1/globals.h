#pragma once
#include "DrawObj.h"
#include "mylog.h"
#define MAX_LOADSTRING 100

// 全域變數: 
class Globals 
{
public:
	HINSTANCE hInst;                                // 目前執行個體
	WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
	WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱
	WCHAR szChildClass[MAX_LOADSTRING];
	HWND hWndFather, myChildWindow;
	HWND myButton[5];
	int currentDrawMode;     //0=line, 1=rect, 2=circle, 3=text, 4=select 
	int modifyState;  //0=new file, 1=modified but not saved, 2=saved or opened
	string fileName;  //file name to display on window title
	DrawObj* pastebinObjectPtr ;  //剪貼簿用的
	DrawObj* selectedObjectPtr;  //選取工具選中的
	DrawObj* preSelectObject;   //顯示游標下方的物件框框用的
	wstring lastFilePath;  //path to last opened file. 

	int mouseX, mouseY;
	TextObj newText;  //only newText needs to be global currently
	bool mouseHasDown;
	bool hasSelected;

	RECT rect;
	bool autoScrolling;
	TCHAR szBuffer[50];
	int cxChar, cyChar;
	list<DrawObj*> drawObjList; // <-next time may use vector<unique_ptr<DrawObj>> DrawObjList;
	mylog mlog;

	//----------------------------------------------------------------------------------------------
	bool debugMode;
	//----------------------------------------------------------------------------------------------

	Globals();
	static Globals& var();
};