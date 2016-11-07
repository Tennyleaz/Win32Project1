#include "stdafx.h"
#include "DrawObj.h"

RectangularObj::RectangularObj() 
{ objectType = 2; }

RectangularObj::~RectangularObj() 
{}

void RectangularObj::Paint(HDC hdc, int Xoffset, int Yoffset) 
{
	if (!startFinished)
		return;
	if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
		return;

	HPEN hPen = switchColor();
	SelectObject(hdc, hPen);
	HBRUSH hBrush = switchBackgroundColor();
	if(hBrush != NULL)
		SelectObject(hdc, hBrush);
	else
		SelectObject(hdc, GetStockObject(NULL_BRUSH));

	int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y);
	int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x);
	int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y);
	int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x);
	Rectangle(hdc, left - Xoffset, top - Yoffset, right - Xoffset, buttom - Yoffset);	

	DeleteObject(hPen);
	if (hBrush != NULL)
		DeleteObject(hBrush);
	releaseColor(hdc);
}

void RectangularObj::PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset)
{
	if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
		return;

	HPEN hpen, hpenOld;
	hpen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
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

	HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(WHITE_BRUSH));
	//draw the 8-points
	{
		//左上
		Rectangle(hdc, left - 4, top - 4, left + 1, top + 1);
		//右上
		Rectangle(hdc, right - 1, top - 4, right + 4, top + 1);
		//左下
		Rectangle(hdc, left - 4, buttom - 1, left + 1, buttom + 4);
		//右下
		Rectangle(hdc, right - 1, buttom - 1, right + 4, buttom + 4);
		//左中
		Rectangle(hdc, left - 4, (buttom + top) / 2 - 3, left + 1, (buttom + top) / 2 + 2);
		//右中
		Rectangle(hdc, right - 1, (buttom + top) / 2 - 3, right + 4, (buttom + top) / 2 + 2);
		//上中
		Rectangle(hdc, (right + left) / 2 - 3, top - 4, (right + left) / 2 + 2, top + 1);
		//下中
		Rectangle(hdc, (right + left) / 2 - 3, buttom - 1, (right + left) / 2 + 2, buttom + 4);
	}
	SelectObject(hdc, oldBrush);
	DeleteObject(oldBrush);
}

bool RectangularObj::CheckObjectCollision(int mouseX, int mouseY)
{
	if (backgroundColor != 0)
	{
		int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y);
		int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x);
		int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y);
		int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x);
		if (mouseY >= top && mouseY <= buttom && mouseX >= left && mouseX <= right)
			return true;
	}
	else if (abs(mouseX - ptBeg.x) <= lineWidth+1 || abs(mouseX - ptEnd.x) <= lineWidth+1 || abs(mouseY - ptBeg.y) <= lineWidth+1 || abs(mouseY - ptEnd.y) <= lineWidth+1)
	{
		//MessageBox(NULL, L"Yes!\nGiven point lies in the RectangularObj", L"Detection", MB_OK);
		return true;
	}
	return false;
}

DrawObj * RectangularObj::clone() const
{
	return new RectangularObj(static_cast<const RectangularObj&>(*this));
}
