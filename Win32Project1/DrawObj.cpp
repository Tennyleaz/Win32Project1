#include "stdafx.h"
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

void DrawObj::PaintMouseOnRect(HDC hdc, int Xoffset, int Yoffset)
{
	if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
		return;

	HPEN hpen, hpenOld;
	HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
	hpen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	hpenOld = (HPEN)SelectObject(hdc, hpen);

	// do something...
	int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y) - Yoffset;
	int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x) - Xoffset;
	int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y) - Yoffset;
	int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x) - Xoffset;
	Rectangle(hdc, left, top, right, buttom);

	//return the pen
	SelectObject(hdc, hpenOld);
	DeleteObject(hpen);
	SelectObject(hdc, oldBrush);
	DeleteObject(oldBrush);
}

void DrawObj::makeStart(int x, int y, int currentColor, int currentBgColor, int currentLineWidth)  //x and y is absolute position on background
{
	ptBeg.x = x;
	ptBeg.y = y;
	ptEnd.x = x;
	ptEnd.y = y;
	startFinished = true;
	endFinished = false;
	color = currentColor;
	backgroundColor = currentBgColor;
	lineWidth = currentLineWidth;
}

void DrawObj::makeEnd(int x, int y, int xCurrentScroll, int yCurrentScroll)  //x and y is related position
{
	ptEnd.x = x + xCurrentScroll;
	ptEnd.y = y + yCurrentScroll;
	endFinished = true;
	//startFinished = false;
}

int DrawObj::CheckMouseIsOnSizingOpint(int mouseX, int mouseY)  
{
	int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y);
	int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x);
	int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y);
	int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x);

	if (mouseY >= top - 4 && mouseY <= top + 1 && mouseX >= left - 4 && mouseX <= left + 1)  //左上
		return 1;
	else if (mouseX >= right - 1 && mouseX <= right + 4 && mouseY >= buttom - 1 && mouseY <= buttom + 4)  //右下
		return 2;
	else if (mouseX >= left - 4 && mouseX <= left + 1 && mouseY >= buttom - 1 && mouseY <= buttom + 4)  //左下
		return 4;
	else if (mouseX >= right - 1 && mouseX <= right + 4 && mouseY >= top - 4 && mouseY <= top + 1) //右上
		return 3;
	else if (mouseX >= left - 4 && mouseX <= left + 1 && mouseY >= (buttom + top) / 2 - 3 && mouseY <= (buttom + top) / 2 + 2)  //左
		return 5;
	else if (mouseX >= right - 1 && mouseX <= right + 4 && mouseY >= (buttom + top) / 2 - 3 && mouseY <= (buttom + top) / 2 + 2)  //右
		return 6;
	else if (mouseX >= (right + left) / 2 - 3 && mouseX <= (right + left) / 2 + 2 && mouseY >= top - 4 && mouseY <= top + 1) //上
		return 7;
	else if (mouseX >= (right + left) / 2 - 3 && mouseX <= (right + left) / 2 + 2 && mouseY >= buttom - 1 && mouseY <= buttom + 4) //下
		return 8;

	return 0;
}

void DrawObj::StartToMove(int mouseX, int mouseY)
{
	originalMouseX = mouseX;
	originalMouseY = mouseY;
	originalBegin = ptBeg;
	originalEnd = ptEnd;
}

void DrawObj::Moving(int mouseX, int mouseY)
{
	int deltaX, deltaY;
	deltaX = mouseX - originalMouseX;
	deltaY = mouseY - originalMouseY;

	ptBeg.x = originalBegin.x + deltaX;
	ptBeg.y = originalBegin.y + deltaY;
	ptEnd.x = originalEnd.x + deltaX;
	ptEnd.y = originalEnd.y + deltaY;

	return;
}

void DrawObj::Resizing(int mouseX, int mouseY, int mode)
{
	int deltaX, deltaY;
	deltaX = mouseX - originalMouseX;
	deltaY = mouseY - originalMouseY;

	//check if resizing is too small
	//if (abs(originalBegin.x - originalEnd.x) < abs(deltaX) - 2)
	//	deltaX = 2 - abs(originalBegin.x - originalEnd.x);

	int beginDeltaX=0, beginDeltaY=0, endDeltaX=0, endDeltaY=0;  //assume begin at upper left...

	switch (mode)
	{
	case 1:
		beginDeltaX = deltaX;
		beginDeltaY = deltaY;
		break;
	case 2:
		endDeltaX = deltaX;
		endDeltaY = deltaY;
		break;
	case 3:  //右上
		endDeltaX = deltaX;
		beginDeltaY = deltaY;		
		break;
	case 4:  //左下
		beginDeltaX = deltaX;
		endDeltaY = deltaY;
		break;
	case 5:  //左
		beginDeltaX = deltaX;
		break;
	case 6:  //右
		endDeltaX = deltaX;
		break;
	case 7:  //上
		beginDeltaY = deltaY;
		break;
	case 8:  //下
		endDeltaY = deltaY;
		break;
	default:
		return;
		break;
	}

	//find which point is upper right
	if (originalBegin.x < originalEnd.x && originalBegin.y < originalEnd.y)  //ptBeg 在左上
	{
		ptBeg.x = originalBegin.x + beginDeltaX;
		ptBeg.y = originalBegin.y + beginDeltaY;
		ptEnd.x = originalEnd.x + endDeltaX;
		ptEnd.y = originalEnd.y + endDeltaY;
		/*if (ptBeg.x > ptEnd.x - 2)
			ptBeg.x = ptEnd.x + 2;
		if (ptBeg.y > ptEnd.y - 2)
			ptBeg.y = ptEnd.y + 2;*/
	}
	else if (originalBegin.x < originalEnd.x && originalBegin.y >= originalEnd.y) //ptBeg 在左下
	{
		ptBeg.x = originalBegin.x + beginDeltaX;
		ptBeg.y = originalBegin.y + endDeltaY;
		ptEnd.x = originalEnd.x + endDeltaX;
		ptEnd.y = originalEnd.y + beginDeltaY;
	}
	else if (originalBegin.x >= originalEnd.x && originalBegin.y < originalEnd.y) //ptBeg 在右上
	{
		ptBeg.x = originalBegin.x + endDeltaX;
		ptBeg.y = originalBegin.y + beginDeltaY;
		ptEnd.x = originalEnd.x + beginDeltaX;
		ptEnd.y = originalEnd.y + endDeltaY;
	}
	else  //ptBeg在右下
	{
		ptBeg.x = originalBegin.x + endDeltaX;
		ptBeg.y = originalBegin.y + endDeltaY;
		ptEnd.x = originalEnd.x + beginDeltaX;
		ptEnd.y = originalEnd.y + beginDeltaY;
	}
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
	SelectObject(hdc, GetStockObject(DC_PEN));
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
}

HBRUSH DrawObj::switchBackgroundColor()
{
	HBRUSH brush;
	switch (backgroundColor)
	{
	case 0:
		//brush = CreateSolidBrush(RGB(255, 255, 255));
		return NULL;
		break;
	case 1:
		brush = CreateSolidBrush(RGB(180, 180, 180));
		break;
	case 2:
		brush = CreateSolidBrush(RGB(255, 0, 0));
		break;
	case 3:
		brush = CreateSolidBrush(RGB(0, 255, 0));
		break;
	case 4:
		brush = CreateSolidBrush(RGB(0, 0, 255));
		break;
	case 5:
		brush = CreateSolidBrush(RGB(0, 255, 255));
		break;
	case 6:
		brush = CreateSolidBrush(RGB(255, 255, 0));
		break;
	case 7:
		brush = CreateSolidBrush(RGB(255, 0, 255));
		break;
	default:
		return NULL;
	}
	return brush;
	//SelectObject(hdc, GetStockObject(NULL_BRUSH)); //to draw a empty rectangle
}

