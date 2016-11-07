#include "stdafx.h"
#include "mylog.h"

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

void mylog::Revert()
{
	json J = ops.back();
	//switch (J[0])
	//{
	//case "add":
	//case "delete":
	//default:
	//	break;
	//}
}

void mylog::Redo()
{
}

void mylog::PushObject(DrawObj* it, json& jit)
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

	//j = jit;
}


