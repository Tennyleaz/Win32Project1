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
	// TODO: �󦹳B���J�Ǧ^���z��
	static globals g;
	return g;
}
