#pragma once
#include "DrawObj.h"

#define MAX_LOADSTRING 100

// �����ܼ�: 
HINSTANCE hInst;                                // �ثe�������
WCHAR szTitle[MAX_LOADSTRING];                  // ���D�C��r
WCHAR szWindowClass[MAX_LOADSTRING];            // �D�������O�W��
WCHAR szChildClass[] = L"123";
HWND hWndFather, myChildWindow;
HWND myButton[5];
int currentDrawMode = 0;     //0=line, 1=rect, 2=circle, 3=text, 4=select 
int modifyState = 0;  //0=new file, 1=modified but not saved, 2=saved or opened
string fileName = "Untitiled";
auto lastStateObject = NULL;  //�W�@�B�Ϊ�����
auto recoverObject = NULL;  //�U�@�B�Ϊ�
auto pastebinObject = NULL;  //�ŶKï�Ϊ�
DrawObj* selectedObject = NULL;  //����u��襤��

static int mouseX, mouseY;
static LineObj  newLine;
static RectangularObj newRect;
static TextObj newText;
static CircleObj newCircle;
static bool mouseHasDown = false;

static RECT rect;
static TCHAR szBuffer[50];
static int cxChar, cyChar;
//int i;
list<DrawObj*> DrawObjList;             // <-this is garbage don't use it
//vector<unique_ptr<DrawObj>> DrawObjList;  // <-use this

//----------------------------------------------------------------------------------------------
//for WinProc only

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
static int currentCursorMode;   //0=original 1=���W 2=�k�U 3=�k�W 4=���U 5=�� 6=�k 7=�W 8=�U 9=�|�V
static bool hasSelected;
static HCURSOR cursors[6];      //0=original 1=���W�k�U 2=�k�W���U 3=���k 4=�W�U 5=�|�V
//string debugmessage = "cursorX=";