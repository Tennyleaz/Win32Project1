#pragma once
#include "json.hpp"
#include "DrawObj.h"
#include <string>
#include <iostream>
#include <fstream>

using json = nlohmann::json;
using namespace std;

int SaveToFile(list<DrawObj>& saveList)
{
	//put each object to json
	json j;
	for (auto& it : saveList)  //Draw each object in DrawObjList
	{
		json jit;
		jit["objectType"] = it.objectType;
		jit["ptBeg"] = { it.ptBeg.x, it.ptBeg.y };
		jit["ptEnd"] = { it.ptEnd.x, it.ptEnd.y };
		jit["color"] = it.color;
		if (it.objectType == 4)
		{
			//TextObj* t = dynamic_cast<TextObj*>(it);
			TextObj *t = new TextObj(*it);
			jit["text"] = t->text;
		}
		cout << "jit = " << jit << endl;
		j.push_back(jit);
	}

	// open a file in write mode.
	ofstream outfile;
	outfile.open("myfile.json");
	outfile << j << endl;
	outfile.close();

	return 0;
}

int ReadFromFile(string fileName, list<DrawObj>& readList)
{
	return 0;
}