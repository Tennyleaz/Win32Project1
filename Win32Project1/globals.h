#pragma once
#include "DrawObj.h"

#define MAX_LOADSTRING 100

// �����ܼ�: 
class globals 
{
public:
	HINSTANCE hInst;                                // �ثe�������
	WCHAR szTitle[MAX_LOADSTRING];                  // ���D�C��r
	WCHAR szWindowClass[MAX_LOADSTRING];            // �D�������O�W��
	WCHAR szChildClass[MAX_LOADSTRING];
	HWND hWndFather, myChildWindow;
	HWND myButton[5];
	int currentDrawMode;     //0=line, 1=rect, 2=circle, 3=text, 4=select 
	int modifyState;  //0=new file, 1=modified but not saved, 2=saved or opened
	string fileName;
	//auto lastStateObject;  //�W�@�B�Ϊ�����
	DrawObj* recoverObjectPtr;  //�U�@�B�Ϊ�
	DrawObj* pastebinObjectPtr ;  //�ŶKï�Ϊ�
	DrawObj* selectedObjectPtr;  //����u��襤��
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