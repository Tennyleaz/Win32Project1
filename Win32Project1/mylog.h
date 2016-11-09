#pragma once
#include "stdafx.h"
#include "json.hpp"
#include <string>
#include <list>
#include "DrawObj.h"

using namespace std;
using json = nlohmann::json;

//operation:
//0=add, 1=delete, 2=move, 3=resize, 4=modify, 5=modify text
class mylog  
{
	list<json> ops;
	void PushObject(DrawObj* d, json j);
	json jmove;
	json jredo;
public:
	mylog();
	~mylog();
	void Commit(json J);
	void Undo();
	void Redo();
	void ClearLogs();

	void OP_add(DrawObj* d);
	void OP_del(DrawObj* d, int pos);
	void OP_moveStart(DrawObj* d, int pos);
	void OP_moveEnd(DrawObj * d);
	void OP_sizeStart(DrawObj* d, int pos);
	void OP_sizeEnd(DrawObj* d);
	void OP_textStart(DrawObj* d, int pos);
	void OP_textEnd(DrawObj* d);
	void OP_modifyStart(DrawObj* d, int pos);
	void OP_modifyEnd(DrawObj* d);
};
