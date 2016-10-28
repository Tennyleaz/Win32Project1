#include "stdafx.h"
#include "DrawObj.h"

TextObj::TextObj()
{
	text.clear();
	text.push_back("");
	objectType = 4;
}

void TextObj::clean()
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

TextObj::~TextObj()
{

}

void TextObj::Paint(HDC hdc, int Xoffset, int Yoffset)
{
	if (!startFinished)
		return;

	string s = "";
	RECT rc;
	SetRect(&rc, ptBeg.x - Xoffset, ptBeg.y - Yoffset, ptBeg.x - Xoffset + 300, ptBeg.y - Yoffset + 300);

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
		s = "*line number " + to_string(text.size()) + ",char number " + (text.size() > 0 ? to_string(text.back().size()) : "NULL");
		TextOutA(hdc, ptBeg.x - Xoffset, ptBeg.y - Yoffset - 13, s.c_str(), s.length());

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	SetTextColor(hdc, RGB(0, 0, 0));
	DeleteObject(hFont);
}

void TextObj::PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset)
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

void TextObj::addChar(int c)
{
	if (text.size() == 0)
		text.push_back("");

	if (c >= 65 && c <= 90)  //A~Z
		text.back().push_back(c + 32);
	else if (c >= 48 && c <= 57)  //0~9
		text.back().push_back(c);
	else if (c >= 0x60 && c <= 0x69) //Numeric keypad 0~9
		text.back().push_back(c - 48);
	else if (c == 0x20)  //space
		text.back().push_back(' ');
}

void TextObj::addNewLine()
{
	if (text.size() == 0)
		text.push_back("");
	text.push_back("");
}

void TextObj::backspace()
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

	if (text.size() == 0)  //size is 0, put a empty one
	{
		text.push_back("");
	}
}

bool TextObj::CheckObjectCollision(int mouseX, int mouseY)
{
	return false;
	//do nothing currently
}