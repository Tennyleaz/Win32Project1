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
	int nObjType;
	int color;
	bool startFinished, endFinished;
	DrawObj()
	{
		color = 0;
		//nObjType = 0;
		startFinished = false;
		endFinished = false;
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
	void makeStart(int x, int y, int currentColor)
	{
		ptBeg.x = x;
		ptBeg.y = y;
		ptEnd.x = x;
		ptEnd.y = y;
		startFinished = true;
		endFinished = false;
		color = currentColor;
	}
	void makeEnd(int x, int y, int xCurrentScroll, int yCurrentScroll)
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
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			break;
		case 1:
			hPen = CreatePen(PS_SOLID, 1, RGB(180, 180, 180));
			break;
		case 2:
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			break;
		case 3:
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			break;
		case 4:
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
			break;
		case 5:
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
			break;
		case 6:
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
			break;
		case 7:
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
			break;
		default:
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
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
	LineObj()	{	}
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
};

class TextObj : public DrawObj
{	
public:
	list<string> text;
	TextObj()
	{
		text.clear(); 
		text.push_back("");
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
			s = "line number " + to_string(text.size()) + ",char number " + (text.size() > 0 ? to_string(text.back().size()):"NULL");
			TextOutA(hdc, ptBeg.x - Xoffset, ptBeg.y - Yoffset - 13, s.c_str(), s.length());
			
			// Restore the original font.        
			SelectObject(hdc, hOldFont);
		}
		SetTextColor(hdc, RGB(0, 0, 0));
		DeleteObject(hFont);
	}
	void addChar(char c)
	{
		if (text.size() == 0)
			text.push_back("");
		text.back().push_back(c);
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
};

class RectangularObj : public DrawObj
{
public:
	RectangularObj() {}
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
};

class CircleObj : public DrawObj
{
	//int xRadius, yRadius;
public:
	CircleObj()	{	}
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
};

