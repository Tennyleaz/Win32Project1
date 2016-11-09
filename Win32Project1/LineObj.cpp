#include "stdafx.h"
#include "DrawObj.h"
LineObj::LineObj() { objectType = 1; }

void LineObj::Paint(HDC hdc, int Xoffset, int Yoffset)
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

void LineObj::PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset)
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

bool LineObj::CheckObjectCollision(int mouseX, int mouseY)
{
	float slope, intercept, epsilon;
	float x1, y1, x2, y2;
	float px, py;
	float left, top, right, bottom; // Bounding Box For Line Segment
	float dx, dy;
	epsilon = 4.0 + lineWidth;

	x1 = ptBeg.x;
	y1 = ptBeg.y;
	x2 = ptEnd.x;
	y2 = ptEnd.y;
	px = mouseX;
	py = mouseY;

	dx = x2 - x1;
	dy = y2 - y1;

	if (dx == 0)  //vertical line
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
		top = y2;
		bottom = y1;
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
		//MessageBox(NULL, L"Given point is outside the line segment", L"Detection", MB_OK);
	return false;
}

DrawObj * LineObj::clone() const
{
	return new LineObj(static_cast<const LineObj&>(*this));
}
