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

void mylog::Undo()
{
	if (ops.empty())
		return;

	json J = ops.back();
	ops.pop_back();
	jredo = J;
	
	if (J.empty())
		return;

	int op = J["operation"];
	switch (op)
	{
	case 0:  //undo add
	{
		//just delete the tail !
		delete globals::var().DrawObjList.back();
		globals::var().DrawObjList.pop_back();
		globals::var().selectedObjectPtr = nullptr;
		globals::var().hasSelected = false;
		break;
	}
	case 1:  //undo delete
	{
		int pos = J["which"];

		if (pos == -1)  //for add back newText
		{
			globals::var().newText.makeStart(J["ptBeg"][0], J["ptBeg"][1], J["color"], J["backgroundColor"], J["lineWidth"]);
			globals::var().newText.makeEnd(J["ptEnd"][0], J["ptEnd"][1], 0, 0);
			vector<string> text = J["text"];
			globals::var().newText.text = text;
			globals::var().newText.inputPos.x = J["inputpos"][0];
			globals::var().newText.inputPos.y = J["inputpos"][1];
			globals::var().newText.CalculateCaretPosition();
			globals::var().newText.startFinished = true;
			globals::var().newText.endFinished = false;

			//select the newText
			globals::var().selectedObjectPtr = &globals::var().newText;
			globals::var().hasSelected = true;
			break;
		}

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
	case 2:  //undo move
	{
		//move back the ptBeg/End
		int pos = J["which"];

		if (pos == -1)  //for moving newText
		{
			int deltaX, deltaY;
			deltaX = J["deltax"];
			deltaY = J["deltay"];
			globals::var().newText.ptBeg.x = J["start"][0];
			globals::var().newText.ptBeg.y = J["start"][1];
			globals::var().newText.ptEnd.x += deltaX;
			globals::var().newText.ptEnd.y += deltaY;
			break;
		}

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
	case 3:  //undo resize
	{
		//roll back to old points
		int pos = J["which"];

		if (pos == -1)  //for newText
		{
			globals::var().newText.ptBeg.x = J["oldBegin"][0];
			globals::var().newText.ptBeg.y = J["oldBegin"][1];
			globals::var().newText.ptEnd.x = J["oldEnd"][0];
			globals::var().newText.ptEnd.y = J["oldEnd"][1];
			break;
		}

		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);

		(*it)->ptBeg.x = J["oldBegin"][0];
		(*it)->ptBeg.y = J["oldBegin"][1];
		(*it)->ptEnd.x = J["oldEnd"][0];
		(*it)->ptEnd.y = J["oldEnd"][1];
		break;
	}
	case 4:  //undo modify
	{
		//revert old color/width
		int pos = J["which"];

		if (pos == -1)  //for newText
		{
			globals::var().newText.color = J["oldColor"];
			globals::var().newText.backgroundColor = J["oldBgColor"];
			break;
		}

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
	}
	case 5:  //undo modify text
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
			UpdateNewText(vs, in);  //just update the newText

		break;
	}
	default:
		break;
	}
}

void mylog::Redo()
{
	json J = jredo;	

	if (J.empty())
		return;

	int op = J["operation"];
	switch (op)
	{
	case 0:  //redo add
	{
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
	case 1:  //redo delete
	{
		//delete the given position
		int pos = J["which"];

		if (pos == -1)  //for newText
		{
			globals::var().newText.clean();
			break;
		}

		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);
		DrawObj * ptr = *it;
		globals::var().DrawObjList.erase(it);
		delete ptr;

		globals::var().selectedObjectPtr = nullptr;
		globals::var().hasSelected = false;
		break;
	}
	case 2:  //redo move
	{
		//move back the ptBeg/End
		int pos = J["which"];
		if (pos == -1)  //for newText
		{
			int deltaX, deltaY;
			deltaX = J["deltax"];
			deltaY = J["deltay"];
			globals::var().newText.ptBeg.x -= deltaX;
			globals::var().newText.ptBeg.y -= deltaY;
			globals::var().newText.ptEnd.x -= deltaX;
			globals::var().newText.ptEnd.y -= deltaY;
			break;
		}

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
	case 3:  //redo resize
	{
		//redo new points
		int pos = J["which"];

		if (pos == -1)  //for newText
		{
			globals::var().newText.ptBeg.x = J["newBegin"][0];
			globals::var().newText.ptBeg.y = J["newBegin"][1];
			globals::var().newText.ptEnd.x = J["newEnd"][0];
			globals::var().newText.ptEnd.y = J["newEnd"][1];
			break;
		}

		auto it = globals::var().DrawObjList.begin();
		std::advance(it, pos);

		(*it)->ptBeg.x = J["newBegin"][0];
		(*it)->ptBeg.y = J["newBegin"][1];
		(*it)->ptEnd.x = J["newEnd"][0];
		(*it)->ptEnd.y = J["newEnd"][1];
		break;
	}
	case 4:  //redo modify
	{
		//redo new points
		int pos = J["which"];
		if (pos == -1)  //for newText
		{
			globals::var().newText.color = J["newColor"];
			globals::var().newText.backgroundColor = J["newBgColor"];
			break;
		}

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
	case 5:  //redo modify text
	{
		string test = J.dump();
		int pos = J["which"];
		vector<string> vs = J["newText"];
		POINT in;
		in.x = J["newInput"][0];
		in.y = J["newInput"][1];

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

	ops.push_back(jredo);  //put the redo operation back to logs !
	jredo.clear();
}

void mylog::ClearLogs()
{
	ops.clear();
	jmove.clear();
	jredo.clear();
	ToggleUndoButton();
}

//push the given object into log list
void mylog::PushObject(DrawObj* it, json jit)
{
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
		jit["inputpos"] = { t->inputPos.x, t->inputPos.y };
	}
	ops.push_back(jit);
	ToggleUndoButton();
}

void mylog::OP_add(DrawObj * it)
{
	json jit;
	jit["operation"] = 0;
	PushObject(it, jit);
}

void mylog::OP_del(DrawObj * it, int pos)
{
	json jit;
	jit["operation"] = 1;
	jit["which"] = pos;
	PushObject(it, jit);
}

//doing before moving
void mylog::OP_moveStart(DrawObj * d, int pos)  //pos = -1 means it is the newText
{
	jmove.clear();
	//know new position of old ptBeg
	jmove["operation"] = 2;
	jmove["start"] = { d->ptBeg.x, d->ptBeg.y };
	jmove["which"] = pos;
}

//doing after moving is end
void mylog::OP_moveEnd(DrawObj * d)
{
	int x = jmove["start"][0];
	int y = jmove["start"][1];
	int dx = x - d->ptBeg.x;
	int dy = y - d->ptBeg.y;
	jmove["deltax"] = dx;  //only log the delta position
	jmove["deltay"] = dy;
	ops.push_back(jmove);
	ToggleUndoButton();
}

//doing before resizing
void mylog::OP_sizeStart(DrawObj * d, int pos)  //pos = -1 meas newText
{
	jmove.clear();
	//know new position of old ptBeg
	jmove["operation"] = 3;
	jmove["oldBegin"] = { d->ptBeg.x, d->ptBeg.y };
	jmove["oldEnd"] = { d->ptEnd.x, d->ptEnd.y };
	jmove["which"] = pos;
}

//doing after sizing is end
void mylog::OP_sizeEnd(DrawObj * d)
{
	jmove["newBegin"] = { d->ptBeg.x, d->ptBeg.y };
	jmove["newEnd"] = { d->ptEnd.x, d->ptEnd.y };
	ops.push_back(jmove);
	ToggleUndoButton();
}

//doing before modify text
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

//doing after modify text
void mylog::OP_textEnd(DrawObj * d)
{
	TextObj* t = dynamic_cast<TextObj*>(d);
	vector<string> vs = t->text;
	jmove["newText"] = vs;
	jmove["newInput"] = { t->inputPos.x, t->inputPos.y };
	ops.push_back(jmove);
	ToggleUndoButton();
	string test = jmove.dump();
}

void mylog::OP_modifyStart(DrawObj * d, int pos)  //if pos = -1 means newText
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
	else //type = text
	{
		jmove["oldBgColor"] = d->backgroundColor;		
	}
}

void mylog::OP_modifyEnd(DrawObj * d)
{
	jmove["newColor"] = d->color;
	if (d->objectType < 4)
	{
		jmove["newWidth"] = d->lineWidth;
	}
	if (d->objectType > 1)
	{
		jmove["newBgColor"] = d->backgroundColor;
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
			modified = true;
		}
		if (d->objectType > 1 && d->objectType < 4)
		{
			if (d->backgroundColor != (int)jmove["oldBgColor"])
			{
				modified = true;
			}
		}
	}
	else  //type = text
	{
		if (d->backgroundColor != (int)jmove["oldBgColor"])
		{
			modified = true;
		}
	}

	if (modified)
	{
		ops.push_back(jmove);
		ToggleUndoButton();
	}
}

