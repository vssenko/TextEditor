
#include "stdafx.h"

Text::Text(void)
{
	data = std::vector<ExtendedChar>();
}

Text::~Text(void)
{
}
int Text:: AddBitmap(HBITMAP bitmap, int pos)
{
	if (bitmap ==NULL)
		return 0;
	images.push_back(bitmap);
	ExtendedChar *bitmpchar = new ExtendedChar();
	bitmpchar->ifImageThenImageIndex = images.size() - 1; // последний индекс
	this->data.insert(data.begin() + pos, *bitmpchar);
	return 1;
}

int Text::AddChar(TCHAR chr, int pos)
{
	ExtendedChar* newChar = new ExtendedChar();
	newChar->chr = chr;
	data.insert(data.begin() + pos, *newChar);
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