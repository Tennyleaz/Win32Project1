#pragma once

#include "Windows.h"
#include <string>
#include <list>
#include <vector>
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
	//virtual DrawObj* clone() const = 0;
	void PaintMouseOnRect(HDC hdc, int Xoffset, int Yoffset);
	void makeStart(int x, int y, int currentColor, int currentBgColor, int currentLineWidth); //x and y is absolute position on background
	void makeEnd(int x, int y, int xCurrentScroll, int yCurrentScroll); //x and y is related position
	int CheckMouseIsOnSizingOpint(int mouseX, int mouseY);	//return 0~4, 0 is normal mouse
	void StartToMove(int mouseX, int mouseY);  //prepare to move or resize the object. x and y is absolute position on background
	virtual void Moving(int mouseX, int mouseY);
	void Resizing(int mouseX, int mouseY, int mode);
protected:
	int originalMouseX, originalMouseY;
	POINT originalBegin, originalEnd;
	HPEN switchColor();
	void releaseColor(HDC hdc);
	HBRUSH switchBackgroundColor();
};

class LineObj : public DrawObj
{
public:
	LineObj();
	virtual ~LineObj() {}
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override;
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) override;
	virtual bool CheckObjectCollision(int mouseX, int mouseY);
	//virtual DrawObj* clone() const;
};

class TextObj : public DrawObj
{
private:
	int textWidth, textHeight, maxTextWidth;
	bool addChar(int c);
	bool addNewLine();
	bool backspace();
	bool del();
	HPEN switchTextBoxLineColor();
public:
	//int textWidth, textHeight, maxTextWidth;
	vector<string> text;
	POINT tailPos;  //tailPos is the last char position of text
	POINT caretPos;
	POINT inputPos;
	TextObj();
	void clean();
	virtual ~TextObj();
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset);
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset);
	virtual bool CheckObjectCollision(int mouseX, int mouseY);
	bool KeyIn(int wParam);  //return false if no change
	void ResizingText(int mouseX, int mouseY, int mode);
	bool CheckTextBoxBigEnough(int X, int Y);
	void CalculateCaretPosition();
	void Moving(int mouseX, int mouseY);
	//virtual DrawObj* clone() const;
};

class RectangularObj : public DrawObj
{
public:
	RectangularObj();
	virtual ~RectangularObj();
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override;
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) override;
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override;
	//virtual DrawObj* clone() const;
};

class CircleObj : public DrawObj
{
	//int xRadius, yRadius;
public:
	CircleObj();
	virtual ~CircleObj();
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override;
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) override;
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override;
	//virtual DrawObj* clone() const;
};
