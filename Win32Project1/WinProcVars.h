#pragma once
#include "stdafx.h"
#include "DrawObj.h"
 
class WinProcVars
{
public:
	/*static int mouseX, mouseY;
	static LineObj  newLine;
	static RectangularObj newRect;
	static TextObj newText;
	static CircleObj newCircle;
	static bool mouseHasDown;

	static RECT rect;
	static TCHAR szBuffer[50];
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
	static HBITMAP checkedIcon;
	static HMENU hMenu;        //try to get the system menu
	static HBITMAP hBmp;          //bitmap for memory DC

	static int currentColor;        //0=black, 0~7 kinds of color
	static int currentBgColor;      //0=transparent, 0~7 kinds of color
	static int currentLineWidth;    //width 1~5
	static int currentCursorMode;   //0=original 1=左上 2=右下 3=右上 4=左下 5=左 6=右 7=上 8=下 9=四向
	static bool hasSelected;
	static HCURSOR cursors[6];      //0=original 1=左上右下 2=右上左下 3=左右 4=上下 5=四向
	//string debugmessage = "cursorX=";*/

	WinProcVars();
	/*static WinProcVars& MyVar(){
		static WinProcVars var;
		return var;
	}*/
};