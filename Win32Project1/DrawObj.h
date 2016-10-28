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
	LineObj();
	virtual ~LineObj() {}
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override;
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) override;
	virtual bool CheckObjectCollision(int mouseX, int mouseY);
};

class TextObj : public DrawObj
{
public:
	list<string> text;
	TextObj();
	void clean();
	virtual ~TextObj();
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset);
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset);
	void addChar(int c);
	void addNewLine();
	void backspace();
	virtual bool CheckObjectCollision(int mouseX, int mouseY);
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
