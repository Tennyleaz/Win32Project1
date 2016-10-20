#pragma once
#include "Windows.h"
#include <string>
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
		nObjType = 0;
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
};

class LineObj : public DrawObj
{
public:
	//POINT start, end;
	LineObj()	{	}
	virtual ~LineObj() {}
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (!startFinished)
			return;
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

		SelectObject(hdc, hPen);

		MoveToEx(hdc, ptBeg.x - Xoffset, ptBeg.y - Yoffset, NULL);
		LineTo(hdc, ptEnd.x - Xoffset, ptEnd.y - Yoffset);

		DeleteObject(hPen);
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(hdc, hPen);
	}
};

class TextObj : public DrawObj
{	
public:
	string text;
	TextObj()
	{
		text = ""; 
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
		text = "";
	}
	virtual ~TextObj() {}
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override
	{
		if (!startFinished)
			return;
		string s = text;
		/*if(endFinished)
			s = text;
		else
			s = "I" + text;*/
		//TextOutA(hdc, ptBeg.x - Xoffset, ptBeg.y - Yoffset, s.c_str(), s.length());
		RECT rc;
		SetRect(&rc, ptBeg.x - Xoffset, ptBeg.y - Yoffset, ptBeg.x - Xoffset+ 300, ptBeg.y - Yoffset + 300);
		DrawTextA(hdc, s.c_str(), s.length(), &rc, DT_LEFT | DT_NOCLIP);
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
		SelectObject(hdc, hPen);

		int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y);
		int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x);
		int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y);
		int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x);
		Rectangle(hdc, left - Xoffset, top - Yoffset, right - Xoffset, buttom - Yoffset);

		DeleteObject(hPen);
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(hdc, hPen);
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
		SelectObject(hdc, hPen);

		int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y);
		int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x);
		int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y);
		int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x);
		Ellipse(hdc, left-Xoffset, top-Yoffset, right-Xoffset, buttom-Yoffset);

		DeleteObject(hPen);
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(hdc, hPen);
	}
};

