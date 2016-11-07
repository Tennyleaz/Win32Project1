#pragma once
#include "stdafx.h"
#include "json.hpp"
#include <string>
#include <list>
#include "DrawObj.h"

using namespace std;
using json = nlohmann::json;

class mylog
{
	static json ops;
public:
	mylog();
	~mylog();
	void Commit(json J);
	void Revert();
	void Redo();
	void PushObject(DrawObj* d, json& j);
};
