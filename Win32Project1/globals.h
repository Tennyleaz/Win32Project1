#pragma once
#include "DrawObj.h"

#define MAX_LOADSTRING 100

// 全域變數: 
class globals 
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
	string fileName;
	//auto lastStateObject;  //上一步用的物件
	DrawObj* recoverObjectPtr;  //下一步用的
	DrawObj* pastebinObjectPtr ;  //剪貼簿用的
	DrawObj* selectedObjectPtr;  //選取工具選中的
	DrawObj* preSelectObject;
	wstring lastFilePath;

	int mouseX, mouseY;
	LineObj  newLine;
	RectangularObj newRect;
	TextObj newText;
	CircleObj newCircle;
	bool mouseHasDown;

	RECT rect;
	TCHAR szBuffer[50];
	int cxChar, cyChar;
	//int i;
	list<DrawObj*> DrawObjList;             // <-this is garbage don't use it
	list<DrawObj*> BackupList;
	//vector<unique_ptr<DrawObj>> DrawObjList;  // <-use this

	//----------------------------------------------------------------------------------------------

	globals();
	static globals& var();
};