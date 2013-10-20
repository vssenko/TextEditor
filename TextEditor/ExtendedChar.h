#pragma once
#include "stdafx.h"
class ExtendedChar
{
public:
	TCHAR chr;
	HFONT font;
	int size;
	int ifImageThenImageIndex;
	bool isBold;
	bool isCursive;
	bool isUnderLine;
	ExtendedChar(void);
	~ExtendedChar(void);
};

