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
	// TODO: �󦹳B���J�Ǧ^���z��
	static globals g;
	return g;
}
