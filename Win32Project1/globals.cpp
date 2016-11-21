#include "stdafx.h"
#include "globals.h"

Globals::Globals()
{
	fileName = "Untitiled";
	currentDrawMode = 0;
	modifyState = 0;
	selectedObjectPtr = NULL;
	preSelectObject = NULL;
	mouseHasDown = false;
	wcsncpy(szChildClass, L"123", 3);  //name for child window class. cannot be null !
	debugMode = false;
}

Globals & Globals::var()
{
	// TODO: 於此處插入傳回陳述式
	static Globals g;
	return g;
}
