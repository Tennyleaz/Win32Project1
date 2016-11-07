#include "stdafx.h"
#include "globals.h"

globals::globals()
{
	fileName = "Untitiled";
	currentDrawMode = 0;
	modifyState = 0;
	selectedObjectPtr = NULL;
	preSelectObject = NULL;
	mouseHasDown = false;
	wcsncpy(szChildClass, L"123", 3);
}

globals & globals::var()
{
	// TODO: 於此處插入傳回陳述式
	static globals g;
	return g;
}
