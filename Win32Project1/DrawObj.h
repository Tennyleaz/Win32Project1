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
	void makeStart(int x, int y, int currentColor, int currentBgColor, int currentLineWidth); //x and y is absolute position on background
	void makeEnd(int x, int y, int xCurrentScroll, int yCurrentScroll); //x and y is related position
	int CheckMouseIsOnSizingOpint(int mouseX, int mouseY);	//return 0~4, 0 is normal mouse
	void StartToMove(int mouseX, int mouseY);  //prepare to move or resize the object. x and y is absolute position on background
	void Moving(int mouseX, int mouseY); 
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
	RectangularObj();
	virtual ~RectangularObj();
	virtual void Paint(HDC hdc, int Xoffset, int Yoffset) override;
	virtual void PaintSelectedRect(HDC hdc, int Xoffset, int Yoffset) override;
	virtual bool CheckObjectCollision(int mouseX, int mouseY) override;
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
};
