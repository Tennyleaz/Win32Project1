#include "stdafx.h"
#include "DrawObj.h"

TextObj::TextObj()
{
	text.clear();
	text.push_back("");
	objectType = 4;
	textHeight = 13 + 1;
	textWidth = 5 * 8 + 1;
	tailPos.x = 0;
	tailPos.y = 0;
}

void TextObj::clean()
{
	ptBeg.x = 0;
	ptBeg.y = 0;
	ptEnd.x = 0;
	ptEnd.y = 0;
	tailPos.x = 0;
	tailPos.y = 0;
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
	hFont = (HFONT)GetStockObject(ANSI_FIXED_FONT);  //目前寬8，高13

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		// Display the text string.  Using a Stock Font to Draw Text.
		int lineSize = textWidth / 8;
		tailPos.y = -13;
		for (auto it : text)
		{
			//add all the strings to one and print it
			int i = 0;
			tailPos.x = 0;
			for (auto it2 : it)
			{
				if (i > 0 && i%lineSize == 0)
				{
					s.push_back('\n');
					tailPos.y += 13;
					tailPos.x = 0;
				}
				s.push_back(it2);
				i++;
				tailPos.x += 8;
			}
			//s += it;
			s += '\n';
			tailPos.y += 13;
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

	textWidth = ptEnd.x - ptBeg.x;
	textHeight = ptEnd.y - ptBeg.y;
	//paint the text block
	/*HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
	int top = ptBeg.y ;
	int left = ptBeg.x;
	int buttom = ptEnd.y;
	int right = ptEnd.x;	
	Rectangle(hdc, left - Xoffset, top - Yoffset, left + textWidth - Xoffset, top + textHeight - Yoffset);
	SelectObject(hdc, oldBrush);
	DeleteObject(oldBrush);*/
	//PaintSelectedRect(hdc, Xoffset, Yoffset);
}

void TextObj::PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset)
{
	if (ptBeg.x == ptEnd.x && ptBeg.y == ptEnd.y)
		return;

	HPEN hpen, hpenOld;
	hpen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	hpenOld = (HPEN)SelectObject(hdc, hpen);
	HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

	// do something...
	int top = ptBeg.y ;
	int left = ptBeg.x;
	int buttom = ptEnd.y;
	int right = ptEnd.x;
	Rectangle(hdc, left - Xoffset, top - Yoffset, right - Xoffset, buttom - Yoffset);

	//return the pen and brush
	SelectObject(hdc, hpenOld);
	SelectObject(hdc, oldBrush);
	DeleteObject(oldBrush);
	DeleteObject(hpen);
	DeleteObject(hpenOld);

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

	if (mouseY >= ptBeg.y && mouseY <= ptEnd.y && mouseX >= ptBeg.x && mouseX <= ptEnd.x)
		return true;
	return false;
	//do nothing currently
}

void TextObj::KeyIn(int wParam)
{
	if ((wParam >= 65 && wParam <= 90) || (wParam >= 48 && wParam <= 57) || (wParam >= 0x60 && wParam <= 0x69) || (wParam == 0x20))
	{
		if ((text.back().size()+1) * 8 > textWidth) //if x > 2000 add new line and add new char
		{
			int newy = ptBeg.y + (text.size() + 1) * 13;
			if (newy < 1990)  //if y < 2000 add new line
			{
				//addNewLine();
			}
			else
				return;  //do nothing
		}
		addChar(wParam);
	}
	else if (wParam == 0x0D)  //enter
	{
		if (ptBeg.y + (text.size() + 1) * 13 < 1988)
		{
			addNewLine();  //insert a "" string to back
		}
	}
	else if (wParam == 0x08)  //backspace <-
	{
		backspace();
	}
	
	//calculate text box
	int lineCount = 0;
	int lineSize = textWidth / 8;
	for (int i = 0; i < text.size(); i++)
	{
		//check each line, see how many chars they have?
		int chars = text[i].size();
		lineCount = lineCount + (chars - 1) / lineSize + 1;
	}

	textHeight = lineCount * 13;

	if (text.size() > 0 && textHeight < text.size() * 13)
		textHeight = text.size() * 13;
	else if(text.size() == 0)
		textHeight = 13;

	ptEnd.x = ptBeg.x + textWidth;
	ptEnd.y = ptBeg.y + textHeight;

}

void TextObj::ResizingText(int mouseX, int mouseY, int mode)
{
	int deltaX, deltaY;
	deltaX = mouseX - originalMouseX;
	deltaY = mouseY - originalMouseY;

	//check if resizing is too small
	//if (abs(originalBegin.x - originalEnd.x) < abs(deltaX) - 2)
	//	deltaX = 2 - abs(originalBegin.x - originalEnd.x);

	int beginDeltaX = 0, beginDeltaY = 0, endDeltaX = 0, endDeltaY = 0;  //assume begin at upper left...

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

	//check the new size is valid or not
	int newWidth, newHeight;
	newWidth = textWidth + deltaX;
	newHeight = textHeight + deltaY;
	//if (!CheckTextBoxBigEnough(newWidth, newHeight))
	//	return;
	if (tailPos.y > newHeight)
		return;

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

bool TextObj::CheckTextBoxBigEnough(int X, int Y)
{
	if (text.size() <= 0)
		return true;

	if (X < 8 || Y < 13)
		return false;

	int lineSize = X / 8;
	int lineCount = Y / 13;

	//if(text.size() == 1 && text.front().size() < 5)

	//take one char from text each time.
	//if a horizontal line is filled with text, delete y by 1;
	for (int i = 0; i < text.size(); i++)
	{
		//check each line, see how many chars they have?
		int chars = text[i].size();
		lineCount = lineCount - (chars - 1) / lineSize;
	}
	if (lineCount < 0)
		return false;
	else
		return true;

}
