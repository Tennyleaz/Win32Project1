#include "DrawObj.h"

DrawObj::DrawObj()
{
	color = 0;
	objectType = 0;
	startFinished = false;
	endFinished = false;
	backgroundColor = 0;
	lineWidth = 1;
}

void DrawObj::clean()
{
	ptBeg.x = 0;
	ptBeg.y = 0;
	ptEnd.x = 0;
	ptEnd.y = 0;
	startFinished = false;
	endFinished = false;
	color = 0;
}

DrawObj::~DrawObj(){ }

void DrawObj::makeStart(int x, int y, int currentColor)  //x and y is absolute position on background
{
	ptBeg.x = x;
	ptBeg.y = y;
	ptEnd.x = x;
	ptEnd.y = y;
	startFinished = true;
	endFinished = false;
	color = currentColor;
}

void DrawObj::makeEnd(int x, int y, int xCurrentScroll, int yCurrentScroll)  //x and y is related position
{
	ptEnd.x = x + xCurrentScroll;
	ptEnd.y = y + yCurrentScroll;
	endFinished = true;
}

HPEN DrawObj::switchColor()
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

void DrawObj::releaseColor(HDC hdc)
{
	/*HPEN hPen = GetStockObject(BLACK_PEN);//CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	DeleteObject(hPen);*/
	GetStockObject(DC_PEN);
}