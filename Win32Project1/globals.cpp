#include "stdafx.h"
#include "globals.h"

globals::globals()
{
	fileName = "Untitiled";
	currentDrawMode = 0;
	modifyState = 0;
	selectedObject = NULL;
	mouseHasDown = false;
}

globals & globals::var()
{
	// TODO: 於此處插入傳回陳述式
	static globals g;
	return g;
}
