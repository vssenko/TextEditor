#pragma once
#include "stdafx.h"

class Text
{
public:
	
	Text(void);
	~Text(void);

	std::vector<ExtendedChar> GetStringText(void);
	std::vector<BITMAP> GetBitmapArray(void);
	int Text::GetTextLength(void);
	void AddBitmap(BITMAP bitmap, int pos);
	void DeleteSymbol(int pos);
	void DeleteSymbol(int pos1, int pos2);
	void AddChar(TCHAR chr, int pos);
private:
	//std::wstring* data;
	std::vector<ExtendedChar> data;
	std::vector<BITMAP> images;
};

