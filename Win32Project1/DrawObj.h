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
	DrawObj();
	void clean();
	virtual ~DrawObj();
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) = 0;
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) = 0;
	virtual bool CheckObjectCollision(int mouseX, int mouseY) = 0;  //x and y is absolute position on background
	void makeStart(int x, int y, int currentColor); //x and y is absolute position on background
	void makeEnd(int x, int y, int xCurrentScroll, int yCurrentScroll); //x and y is related position
	
protected:
	HPEN switchColor();
	void releaseColor(HDC hdc);
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
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
			return;
		HPEN hpen, hpenOld;
		hpen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
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
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override
	{
		float slope, intercept, epsilon;
		float x1, y1, x2, y2;
		float px, py;
		float left, top, right, bottom; // Bounding Box For Line Segment
		float dx, dy;
		epsilon = 5.0;

		x1 = ptBeg.x;
		y1 = ptBeg.y;
		x2 = ptEnd.x;
		y2 = ptEnd.y;
		px = mouseX;
		py = mouseY;

		dx = x2 - x1;
		dy = y2 - y1;

		if (dx == 0)
		{
			//only check Y coordinate
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

			if (px == x1 && py >= top && py <= bottom)
			{
				//MessageBox(NULL, L"Yes!\nGiven point lies in the line segment", L"Detection", MB_OK);
				return true;
			}
			else
			{
				//MessageBox(NULL, L"Given point is outside the line segment", L"Detection", MB_OK);
				return false;
			}

		}

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
				//MessageBox(NULL, L"Yes!\nGiven point lies in the line segment", L"Detection", MB_OK);
				return true;
			}
			//else
				//MessageBox(NULL, L"Given point is outside the line segment", L"Detection", MB_OK);
				//std::cout << "Given point is outside the line segment\n";
		}
		else
			//MessageBox(NULL, L"Given point is outside the line segment", L"Detection", MB_OK);
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
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
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
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
			return;

		HPEN hpen, hpenOld;
		hpen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
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
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override
	{
		if (abs(mouseX-ptBeg.x)<=1 || abs(mouseX-ptEnd.x)<=1 || abs(mouseY-ptBeg.y)<=1 || abs(mouseY-ptEnd.y)<=1)
		{
			//MessageBox(NULL, L"Yes!\nGiven point lies in the RectangularObj", L"Detection", MB_OK);
			return true;
		}
		return false;
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
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
			return;

		HPEN hpen, hpenOld;
		hpen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
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
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override
	{
		//(x-h)^2/a^2 + (y-k)^2/b^2 = 1
		int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y);
		int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x);
		int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y);
		int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x);

		float a = (float)(right - left) / 2.0;
		float b = (float)(buttom - top) / 2.0;

		float h = (float)(right + left) / 2.0;
		float k = (float)(buttom + top) / 2.0;

		float result = pow(mouseX - h, 2) / pow(a, 2) + pow(mouseY - k, 2) / pow(b, 2);
		float delta = 0.1;

		if (result <= 1 + delta && result >= 1 - delta)
		{
			//MessageBox(NULL, L"Yes!\nGiven point lies in the ellipse.", L"Detection", MB_OK);
			return true;
		}

		return false;
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
	
	}
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) override
	{
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
