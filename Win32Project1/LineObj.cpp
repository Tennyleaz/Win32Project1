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
	int top = (ptBeg.y < ptEnd.y ? ptBeg.y : ptEnd.y);
	int left = (ptBeg.x < ptEnd.x ? ptBeg.x : ptEnd.x);
	int buttom = (ptBeg.y > ptEnd.y ? ptBeg.y : ptEnd.y);
	int right = (ptBeg.x > ptEnd.x ? ptBeg.x : ptEnd.x);
	Rectangle(hdc, left - Xoffset, top - Yoffset, right - Xoffset, buttom - Yoffset);

	//return the pen
	SelectObject(hdc, hpenOld);
	DeleteObject(hpen);
}

bool LineObj::CheckObjectCollision(int mouseX, int mouseY)
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