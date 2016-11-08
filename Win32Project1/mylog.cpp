#include "stdafx.h"
#include "mylog.h"
#include "globals.h"
#include "WM_Command.h"

mylog::mylog()
{

}

mylog::~mylog()
{
}

void mylog::Commit(json J)
{
	ops.push_back(J);
}

void mylog::Undo()
{
	if (ops.empty())
	{
		MessageBox(NULL, L"JSON list is empty!", L"ERROR", MB_OK);
		return;
	}

	json J = ops.back();
	ops.pop_back();
	jredo = J;
	
	if (J.empty())
	{
		MessageBox(NULL, L"JSON is empty!", L"ERROR", MB_OK);
		return;
	}

	int op = J["operation"];
	switch (op)
	{
	case 0:  //add
	{
		//just delete the tail !
		delete globals::var().DrawObjList.back();
		globals::var().DrawObjList.pop_back();
		globals::var().selectedObjectPtr = nullptr;
		globals::var().hasSelected = false;
		break;
	}
	case 1:  //delete
	{
		int pos = J["which"];
		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);
		//add back the object in the position
		switch ((int)J["objectType"])
		{
		case 1:
		{
			LineObj newLine;
			newLine.makeStart(J["ptBeg"][0], J["ptBeg"][1], J["color"], J["backgroundColor"], J["lineWidth"]);
			newLine.makeEnd(J["ptEnd"][0], J["ptEnd"][1], 0, 0);
			globals::var().DrawObjList.insert(it, new LineObj(newLine));
			break;
		}
		case 2:
		{
			RectangularObj newRect;
			newRect.makeStart(J["ptBeg"][0], J["ptBeg"][1], J["color"], J["backgroundColor"], J["lineWidth"]);
			newRect.makeEnd(J["ptEnd"][0], J["ptEnd"][1], 0, 0);
			globals::var().DrawObjList.insert(it, new RectangularObj(newRect));
			break;
		}
		case 3:
		{
			CircleObj newCircle;
			newCircle.makeStart(J["ptBeg"][0], J["ptBeg"][1], J["color"], J["backgroundColor"], J["lineWidth"]);
			newCircle.makeEnd(J["ptEnd"][0], J["ptEnd"][1], 0, 0);
			globals::var().DrawObjList.insert(it, new CircleObj(newCircle));
			break;
		}
		case 4:
		{
			TextObj newText;
			newText.makeStart(J["ptBeg"][0], J["ptBeg"][1], J["color"], J["backgroundColor"], J["lineWidth"]);
			newText.makeEnd(J["ptEnd"][0], J["ptEnd"][1], 0, 0);
			vector<string> text = J["text"];
			newText.text = text;
			globals::var().DrawObjList.insert(it, new TextObj(newText));
			break;
		}
		}
		break;
	}
	case 2:  //move
	{
		//move back the ptBeg/End
		int pos = J["which"];
		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);

		int deltaX, deltaY;
		deltaX = J["deltax"];
		deltaY = J["deltay"];
		(*it)->ptBeg.x = J["start"][0];
		(*it)->ptBeg.y = J["start"][1];
		(*it)->ptEnd.x += deltaX;
		(*it)->ptEnd.y += deltaY;
		break;
	}
	case 3:  //resize
	{
		//roll back to old points
		int pos = J["which"];
		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);

		(*it)->ptBeg.x = J["oldBegin"][0];
		(*it)->ptBeg.y = J["oldBegin"][1];
		(*it)->ptEnd.x = J["oldEnd"][0];
		(*it)->ptEnd.y = J["oldEnd"][1];
		break;
	}
	case 4:  //modify
	{
		//revert old color/width
		int pos = J["which"];
		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);

		(*it)->color = J["oldColor"];
		if ((*it)->objectType < 4)
		{
			(*it)->lineWidth = J["oldWidth"];
			if ((*it)->objectType > 1)
			{
				(*it)->backgroundColor = J["oldBgColor"];

			}
		}
		break;
		break;
	}
	case 5:  //modify text
	{
		int pos = J["which"];
		vector<string> vs = J["oldText"];
		POINT in;
		in.x = J["oldInput"][0];
		in.y = J["oldInput"][1];

		if (pos != -1)
		{
			auto it = globals::var().DrawObjList.begin();
			std::advance(it, pos);
			TextObj* t = dynamic_cast<TextObj*>((*it));
			t->text = vs;
			t->inputPos = in;
			t->CalculateCaretPosition();
		}
		else
			UpdateNewText(vs, in);

		break;
	}
	default:
		break;
	}
}

void mylog::Redo()
{
	json J = jredo;
	jredo.clear();

	if (J.empty())
	{
		MessageBox(NULL, L"JSON is empty!", L"ERROR", MB_OK);
		return;
	}

	int op = J["operation"];
	switch (op)
	{
	case 0:  //add
	{
		//int pos = J["which"];
		//auto it = globals::var().DrawObjList.begin();
		//std::advance(it, pos);
		//add back the object in the position
		switch ((int)J["objectType"])
		{
		case 1:
		{
			LineObj newLine;
			newLine.makeStart(J["ptBeg"][0], J["ptBeg"][1], J["color"], J["backgroundColor"], J["lineWidth"]);
			newLine.makeEnd(J["ptEnd"][0], J["ptEnd"][1], 0, 0);
			globals::var().DrawObjList.push_back(new LineObj(newLine));
			break;
		}
		case 2:
		{
			RectangularObj newRect;
			newRect.makeStart(J["ptBeg"][0], J["ptBeg"][1], J["color"], J["backgroundColor"], J["lineWidth"]);
			newRect.makeEnd(J["ptEnd"][0], J["ptEnd"][1], 0, 0);
			globals::var().DrawObjList.push_back(new RectangularObj(newRect));
			break;
		}
		case 3:
		{
			CircleObj newCircle;
			newCircle.makeStart(J["ptBeg"][0], J["ptBeg"][1], J["color"], J["backgroundColor"], J["lineWidth"]);
			newCircle.makeEnd(J["ptEnd"][0], J["ptEnd"][1], 0, 0);
			globals::var().DrawObjList.push_back(new CircleObj(newCircle));
			break;
		}
		case 4:
		{
			TextObj newText;
			newText.makeStart(J["ptBeg"][0], J["ptBeg"][1], J["color"], J["backgroundColor"], J["lineWidth"]);
			newText.makeEnd(J["ptEnd"][0], J["ptEnd"][1], 0, 0);
			vector<string> text = J["text"];
			newText.text = text;
			globals::var().DrawObjList.push_back(new TextObj(newText));
			break;
		}
		}
		break;
	}
	case 1:  //delete
	{
		//delete the given position
		int pos = J["which"];
		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);
		DrawObj * ptr = *it;
		globals::var().DrawObjList.erase(it);
		delete ptr;

		//delete globals::var().DrawObjList.back();
		globals::var().selectedObjectPtr = nullptr;
		globals::var().hasSelected = false;
		break;
	}
	case 2:  //move
	{
		//move back the ptBeg/End
		int pos = J["which"];
		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);

		int deltaX, deltaY;
		deltaX = J["deltax"];
		deltaY = J["deltay"];
		(*it)->ptBeg.x -= deltaX;
		(*it)->ptBeg.y -= deltaY;
		(*it)->ptEnd.x -= deltaX;
		(*it)->ptEnd.y -= deltaY;
		break;
	}
	case 3:  //resize
	{
		//redo new points
		int pos = J["which"];
		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);

		(*it)->ptBeg.x = J["newBegin"][0];
		(*it)->ptBeg.y = J["newBegin"][1];
		(*it)->ptEnd.x = J["newEnd"][0];
		(*it)->ptEnd.y = J["newEnd"][1];
		break;
	}
	case 4:  //modify
	{
		//redo new points
		int pos = J["which"];
		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);
		string test = J.dump();
		(*it)->color = J["newColor"];
		if((*it)->objectType < 4)
		{
			(*it)->lineWidth = J["newWidth"];
			if ((*it)->objectType > 1)
			{
				(*it)->backgroundColor = J["newBgColor"];

			}
		}
		break;
	}
	case 5:  //modify text
	{
		int pos = J["which"];
		vector<string> vs = J["newText"];
		POINT in;
		in.x = J["newInput"][0];
		in.x = J["newInput"][1];

		if (pos != -1)
		{
			auto it = globals::var().DrawObjList.begin();
			std::advance(it, pos);
			TextObj* t = dynamic_cast<TextObj*>((*it));
			t->text = vs;
			t->inputPos = in;
			t->CalculateCaretPosition();
		}
		else
			UpdateNewText(vs, in);
		break;
	}
	default:
		break;
	}
}

void mylog::ClearLogs()
{
	ops.clear();
	jmove.clear();
	jredo.clear();
	ToggleUndoButton();
}

void mylog::PushObject(DrawObj* it, json jit)
{
	//json jit;
	jit["objectType"] = it->objectType;
	jit["ptBeg"] = { it->ptBeg.x, it->ptBeg.y };
	jit["ptEnd"] = { it->ptEnd.x, it->ptEnd.y };
	jit["color"] = it->color;
	jit["backgroundColor"] = it->backgroundColor;
	jit["lineWidth"] = it->lineWidth;
	if (it->objectType == 4)
	{
		TextObj* t = dynamic_cast<TextObj*>(it);
		vector<string> ls = t->text;
		jit["text"] = ls;
	}
	ops.push_back(jit);
	ToggleUndoButton();
}

void mylog::OP_add(DrawObj * it)
{
	json jit;
	jit["operation"] = 0;
	PushObject(it, jit);

	//string test = jit.dump();
}

void mylog::OP_del(DrawObj * it, int pos)
{
	json jit;
	jit["operation"] = 1;
	jit["which"] = pos;
	PushObject(it, jit);
}

void mylog::OP_moveStart(DrawObj * d, int pos)
{
	jmove.clear();
	//know new position of old ptBeg
	jmove["operation"] = 2;
	jmove["start"] = { d->ptBeg.x, d->ptBeg.y };
	jmove["which"] = pos;
	//string test = jmove.dump();
}

void mylog::OP_moveEnd(DrawObj * d)
{
	//string test = jmove.dump();
	int x = jmove["start"][0];
	int y = jmove["start"][1];
	int dx = x - d->ptBeg.x;
	int dy = y - d->ptBeg.y;
	jmove["deltax"] = dx;
	jmove["deltay"] = dy;
	//jmove["end"] = { d->ptEnd.x, d->ptEnd.y };
	//test = jmove.dump();
	ops.push_back(jmove);
	ToggleUndoButton();
}

void mylog::OP_sizeStart(DrawObj * d, int pos)
{
	jmove.clear();
	//know new position of old ptBeg
	jmove["operation"] = 3;
	jmove["oldBegin"] = { d->ptBeg.x, d->ptBeg.y };
	jmove["oldEnd"] = { d->ptEnd.x, d->ptEnd.y };
	jmove["which"] = pos;
}

void mylog::OP_sizeEnd(DrawObj * d)
{
	jmove["newBegin"] = { d->ptBeg.x, d->ptBeg.y };
	jmove["newEnd"] = { d->ptEnd.x, d->ptEnd.y };
	ops.push_back(jmove);
	ToggleUndoButton();
}

void mylog::OP_textStart(DrawObj * d, int pos)  //pos = -1 means it is the newText
{
	jmove.clear();
	jmove["operation"] = 5;

	TextObj* t = dynamic_cast<TextObj*>(d);
	vector<string> vs = t->text;
	jmove["oldText"] = vs;
	jmove["oldInput"] = { t->inputPos.x, t->inputPos.y };
	jmove["which"] = pos;
}

void mylog::OP_textEnd(DrawObj * d)
{
	TextObj* t = dynamic_cast<TextObj*>(d);
	vector<string> vs = t->text;
	jmove["newText"] = vs;
	jmove["newInput"] = { t->inputPos.x, t->inputPos.y };
	ops.push_back(jmove);
	ToggleUndoButton();
}

void mylog::OP_modifyStart(DrawObj * d, int pos)
{
	jmove.clear();
	jmove["operation"] = 4;
	jmove["oldColor"] = d->color;
	jmove["which"] = pos;
	if (d->objectType < 4)
	{
		jmove["oldWidth"] = d->lineWidth;
		if (d->objectType > 1)
		{
			jmove["oldBgColor"] = d->backgroundColor;
		}
	}
}

void mylog::OP_modifyEnd(DrawObj * d)
{
	jmove["newColor"] = d->color;
	if (d->objectType < 4)
	{
		jmove["newBgColor"] = d->backgroundColor;
		jmove["newWidth"] = d->lineWidth;
	}

	//if no modify, don't push
	bool modified = false;
	if (d->color != (int)jmove["oldColor"])
	{
		jmove["newColor"] = d->color;
		modified = true;
	}

	if (d->objectType < 4)
	{
		if (d->lineWidth != (int)jmove["oldWidth"])
		{
			//jmove["newWidth"] = d->lineWidth;
			modified = true;
		}
		if (d->objectType > 1 && d->objectType < 4)
		{
			if (d->backgroundColor != (int)jmove["oldBgColor"])
			{
				//jmove["newBgColor"] = d->backgroundColor;
				modified = true;
			}
		}
	}

	if (modified)
	{
		ops.push_back(jmove);
		ToggleUndoButton();
	}
}

