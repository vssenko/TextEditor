
#include "stdafx.h"
Text::Text(void)
{
	data = std::vector<ExtendedChar>();
}
int Text::AddChar(TCHAR chr, HFONT font, int pos)
{
	ExtendedChar extchr = ExtendedChar();
	extchr.chr = chr;
	extchr.font = font;
	data.insert(data.begin() + pos, extchr);
}
Text::~Text(void)
{
}
int Text:: AddBitmap(HBITMAP bitmap, int pos)
{
	if (bitmap ==NULL)
		return 0;
	ExtendedChar bitmpchar =ExtendedChar();
	bitmpchar.bmp = bitmap;
	this->data.insert(data.begin() + pos, bitmpchar);
	return 1;
}
int Text::DeleteSymbol(int pos)
{
	data.erase(data.begin() + pos);
	return 1;
}
int Text::DeleteSymbol(int pos1,int pos2)
{
	data.erase(data.begin() + pos1,data.begin() + pos2);
	return 1;
}