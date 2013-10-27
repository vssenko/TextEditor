#pragma once
#include "stdafx.h"

class Text
{
public:
	
	Text(void);
	~Text(void);

	std::vector<ExtendedChar> GetStringText(void);
	std::vector<HBITMAP> GetBitmapArray(void);
	int Text::GetTextLength(void);
	int AddBitmap(HBITMAP bitmap, int pos);
	int DeleteSymbol(int pos);
	int DeleteSymbol(int pos1, int pos2);
	int AddChar(TCHAR chr, int pos);
private:
	//std::wstring* data;
	std::vector<ExtendedChar> data;
	std::vector<HBITMAP> images;
};

