#pragma once
#include "Windows.h"
#include <string>
#include <list>
using namespace std;

class DrawObj
{
public:
	POINT ptBeg;
	POINT ptEnd;
	int objectType;  //1=line 2=rect 3=circle 4=text 0=undefined!
	int color;
	int backgroundColor;  //0=transparent
	int lineWidth; //1~5
	bool startFinished, endFinished;
	DrawObj()
	{
		color = 0;
		objectType = 0;
		startFinished = false;
		endFinished = false;
		backgroundColor = 0;
		lineWidth = 1;
	}
	void clean()
	{
		ptBeg.x = 0;
		ptBeg.y = 0;
		ptEnd.x = 0;
		ptEnd.y = 0;
		startFinished = false;
		endFinished = false;
		color = 0;
	}
	virtual ~DrawObj() {};
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) = 0;
	virtual bool CheckObjectCollision(int mouseX, int mouseY) = 0;  //x and y is absolute position on background
	void makeStart(int x, int y, int currentColor)  //x and y is absolute position on background
	{
		ptBeg.x = x;
		ptBeg.y = y;
		ptEnd.x = x;
		ptEnd.y = y;
		startFinished = true;
		endFinished = false;
		color = currentColor;
	}
	void makeEnd(int x, int y, int xCurrentScroll, int yCurrentScroll)  //x and y is related position
	{
		ptEnd.x = x + xCurrentScroll;
		ptEnd.y = y + yCurrentScroll;
		endFinished = true;
	}
	
protected:
	HPEN switchColor()
	{
		HPEN hPen;
		switch (color)
		{
		case 0:
			hPen = CreatePen(PS_SOLID, lineWidth, RGB(0, 0, 0));
			break;
		case 1:
			hPen = CreatePen(PS_SOLID, lineWidth, RGB(180, 180, 180));
			break;
		case 2:
			hPen = CreatePen(PS_SOLID, lineWidth, RGB(255, 0, 0));
			break;
		case 3:
			hPen = CreatePen(PS_SOLID, lineWidth, RGB(0, 255, 0));
			break;
		case 4:
			hPen = CreatePen(PS_SOLID, lineWidth, RGB(0, 0, 255));
			break;
		case 5:
			hPen = CreatePen(PS_SOLID, lineWidth, RGB(0, 255, 255));
			break;
		case 6:
			hPen = CreatePen(PS_SOLID, lineWidth, RGB(255, 255, 0));
			break;
		case 7:
			hPen = CreatePen(PS_SOLID, lineWidth, RGB(255, 0, 255));
			break;
		default:
			hPen = CreatePen(PS_SOLID, lineWidth, RGB(0, 0, 0));
		}
		return hPen;
	}
	void releaseColor(HDC hdc)
	{
		/*HPEN hPen = GetStockObject(BLACK_PEN);//CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(hdc, hPen);
		DeleteObject(hPen);*/
		GetStockObject(DC_PEN);
	}
};

class LineObj : public DrawObj
{
public:
	LineObj() { objectType = 1; }
	virtual ~LineObj() {}
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (!startFinished)
			return;
		if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
			return;
		HPEN hPen = switchColor();
		SelectObject(hdc, hPen);

		MoveToEx(hdc, ptBeg.x - Xoffset, ptBeg.y - Yoffset, NULL);
		LineTo(hdc, ptEnd.x - Xoffset, ptEnd.y - Yoffset);

		
		DeleteObject(hPen);
		releaseColor(hdc);
	}
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override
	{
		float slope, intercept, epsilon;
		float x1, y1, x2, y2;
		float px, py;
		float left, top, right, bottom; // Bounding Box For Line Segment
		float dx, dy;
		epsilon = 1.0;

		x1 = ptBeg.x;
		y1 = ptBeg.y;
		x2 = ptEnd.x;
		y2 = ptEnd.y;
		px = mouseX;
		py = mouseY;

		dx = x2 - x1;
		dy = y2 - y1;
		slope = dy / dx;

		// y = mx + c
		// intercept c = y - mx

		intercept = y1 - slope * x1; // which is same as y2 - slope * x2

		// For Bounding Box
		if (x1 < x2)
		{
			left = x1;
			right = x2;
		}
		else
		{
			left = x2;
			right = x1;
		}

		if (y1 < y2)
		{
			top = y1;
			bottom = y2;
		}
		else
		{
			top = y1;
			bottom = y2;
		}

		//std::cout << "Equation of the line: ";
		//std::cout << slope << "X " << ((intercept < 0) ? ' ' : '+') << intercept << "\n";

		if (slope * px + intercept > (py - epsilon) &&
			slope * px + intercept < (py + epsilon))
		{
			if (px >= left && px <= right &&
				py >= top && py <= bottom)
			{
				//std::cout << "Given point lies in the line segment\n";
				MessageBox(NULL, L"Given point lies in the line segment", L"Detection", MB_OK);
				return true;
			}
			else
				MessageBox(NULL, L"Given point is outside the line segment", L"Detection", MB_OK);
				//std::cout << "Given point is outside the line segment\n";
		}
		else
			MessageBox(NULL, L"Given point is outside the line segment", L"Detection", MB_OK);
		return false;
	}
};

class TextObj : public DrawObj
{	
public:
	list<string> text;
	TextObj()
	{
		text.clear(); 
		text.push_back("");
		objectType = 4;
	}
	void clean()
	{
		ptBeg.x = 0;
		ptBeg.y = 0;
		ptEnd.x = 0;
		ptEnd.y = 0;
		startFinished = false;
		endFinished = false;
		color = 0;
		text.clear();
		text.push_back("");
	}
	virtual ~TextObj() {}
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (!startFinished)
			return;

		string s = "";
		RECT rc;
		SetRect(&rc, ptBeg.x - Xoffset, ptBeg.y - Yoffset, ptBeg.x - Xoffset+ 300, ptBeg.y - Yoffset + 300);		

		switch (color)
		{
		case 0:
			SetTextColor(hdc, RGB(0, 0, 0));
			break;
		case 1:
			SetTextColor(hdc, RGB(180, 180, 180));
			break;
		case 2:
			SetTextColor(hdc, RGB(255, 0, 0));
			break;
		case 3:
			SetTextColor(hdc, RGB(0, 255, 0));
			break;
		case 4:
			SetTextColor(hdc, RGB(0, 0, 255));
			break;
		case 5:
			SetTextColor(hdc, RGB(0, 255, 255));
			break;
		case 6:
			SetTextColor(hdc, RGB(255, 255, 0));
			break;
		case 7:
			SetTextColor(hdc, RGB(255, 0, 255));
			break;
		default:
			SetTextColor(hdc, RGB(0, 0, 0));
		}

		// Retrieve a handle to the variable stock font. 
		HFONT hFont, hOldFont;
		hFont = (HFONT)GetStockObject(ANSI_FIXED_FONT);  //¥Ø«e¼e8¡A°ª13

		// Select the variable stock font into the specified device context. 
		if (hOldFont = (HFONT)SelectObject(hdc, hFont))
		{
			// Display the text string.  Using a Stock Font to Draw Text.
			for (auto it : text)
			{				
				s += it;  //add all the strings to one and print it
				s += '\n';
			}
			DrawTextA(hdc, s.c_str(), s.length(), &rc, DT_LEFT | DT_NOCLIP);

			/*SIZE size;
			GetTextExtentPoint32A(hdc, s.c_str(), s.length(), &size);
			s = "string size=" + to_string(size.cx) + ", " + to_string(size.cy);*/
			s = "*line number " + to_string(text.size()) + ",char number " + (text.size() > 0 ? to_string(text.back().size()):"NULL");
			TextOutA(hdc, ptBeg.x - Xoffset, ptBeg.y - Yoffset - 13, s.c_str(), s.length());
			
			// Restore the original font.        
			SelectObject(hdc, hOldFont);
		}
		SetTextColor(hdc, RGB(0, 0, 0));
		DeleteObject(hFont);
	}
	void addChar(int c)
	{
		if (text.size() == 0)
			text.push_back("");

		if (c >= 65 && c <= 90)  //A~Z
			text.back().push_back(c + 32);
		else if(c >= 48 && c <= 57)  //0~9
			text.back().push_back(c);  
		else if (c >= 0x60 && c <= 0x69) //Numeric keypad 0~9
			text.back().push_back(c - 48);
		else if (c == 0x20)  //space
			text.back().push_back(' ');
	}
	void addNewLine()
	{
		if (text.size() == 0)
			text.push_back("");
		text.push_back("");
	}
	void backspace()
	{
		if (text.size() > 0)
		{
			if (text.back().size() > 0)  //string is not empty
			{
				text.back().pop_back();
			}
			else
			{
				text.pop_back();
			}
		}
		
		if(text.size() ==0)  //size is 0, put a empty one
		{
			text.push_back("");
		}
	}
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override
	{
		return false;
		//do nothing currently
	}
};

class RectangularObj : public DrawObj
{
public:
	RectangularObj() { objectType = 2; }
	virtual ~RectangularObj() {}
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (!startFinished)
			return;
		if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
			return;
		HPEN hPen = switchColor();
		SelectObject(hdc, hPen);

		int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y);
		int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x);
		int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y);
		int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x);
		Rectangle(hdc, left - Xoffset, top - Yoffset, right - Xoffset, buttom - Yoffset);

		DeleteObject(hPen);
		releaseColor(hdc);
	}
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override
	{
		return false;
		//do nothing currently
	}
};

class CircleObj : public DrawObj
{
	//int xRadius, yRadius;
public:
	CircleObj() { objectType = 3; }
	virtual ~CircleObj() {}
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (!startFinished)
			return;
		if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
			return;
		HPEN hPen = switchColor();
		SelectObject(hdc, hPen);

		int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y);
		int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x);
		int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y);
		int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x);
		Ellipse(hdc, left-Xoffset, top-Yoffset, right-Xoffset, buttom-Yoffset);

		DeleteObject(hPen);
		releaseColor(hdc);
	}
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override
	{
		return false;
		//do nothing currently
	}
};

class SelectedRect : public DrawObj
{
public:
	bool shown;
	SelectedRect() 
	{ 
		objectType = 0; 
		shown = false;
	}
	virtual ~SelectedRect() {}
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
			return;
		if (!shown)
			return;

		HPEN hpen, hpenOld;
		hpen = CreatePen(PS_DASH, 1, RGB(255, 255, 255));
		hpenOld = (HPEN)SelectObject(hdc, hpen); 
		
		// do something...
		int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y);
		int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x);
		int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y);
		int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x);
		Rectangle(hdc, left - Xoffset, top - Yoffset, right - Xoffset, buttom - Yoffset);

		//return the pen
		SelectObject(hdc, hpenOld);
		DeleteObject(hpen);		
	}
	void Create(POINT pBeg, POINT pEnd)
	{
		SelectedRect::ptBeg = pBeg;
		SelectedRect::ptEnd = pEnd;
	}
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override
	{
		return false;
		//do nothing currently
	}
};