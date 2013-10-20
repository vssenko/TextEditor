
#include "stdafx.h"

Text::Text(void)
{
	data = std::vector<ExtendedChar>();
}

Text::~Text(void)
{
}

std::vector<ExtendedChar> Text::GetStringText(void)
{
	return data;
}

int Text::GetTextLength(void)
{
	return (int)data.size();
}

std::vector<BITMAP> Text::GetBitmapArray()
{
	return images;
}

void Text:: AddBitmap(BITMAP bitmap)
{
	images.push_back(bitmap);
}

void Text::AddChar(TCHAR chr)
{
	ExtendedChar* newChar = new ExtendedChar();
	newChar->chr = chr;
	data.push_back(*newChar);

}
