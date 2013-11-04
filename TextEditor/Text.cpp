
#include "stdafx.h"
Text::Text(AllWhatYouWantController* contr)
{
	data = std::vector<ExtendedChar>();
	father = contr;
}
Text::~Text(void)
{
}
int Text::AddChar(TCHAR chr, HFONT font, int pos)
{
	ExtendedChar extchr = ExtendedChar();
	extchr.chr = chr;
	extchr.font = font;
	data.insert(data.begin() + pos, extchr);
	father->historycontrol->AddState();
	return 1;
}
int Text:: AddBitmap(HBITMAP bitmap, int pos)
{
	if (bitmap ==NULL)
		return 0;
	ExtendedChar bitmpchar =ExtendedChar();
	bitmpchar.bmp = bitmap;
	this->data.insert(data.begin() + pos, bitmpchar);
	father->historycontrol->AddState();
	return 1;
}
int Text::DeleteSymbol(int pos)
{
	data.erase(data.begin() + pos);
	father->historycontrol->AddState();
	return 1;
}
int Text::DeleteSymbol(int pos1,int pos2)
{
	data.erase(data.begin() + min(pos1,pos2),data.begin() + max(pos1,pos2));
	father->historycontrol->AddState();
	return 1;
}
int Text::GetData(std::vector<ExtendedChar>* vectr)
{
	*vectr = data;
	return 1;
}
int Text::AddVectorExtendedChar(std::vector<ExtendedChar> vctr, int pos)
{
	for(int i = 0; i < vctr.size(); i++)
	{
		ExtendedChar chr = vctr[i];
		data.insert(data.begin() + pos, chr);
		pos++;
	}
	return 1;
}
int Text::Move(int firstPos,int secondPos, int positionToMove)
{
	if ((positionToMove >= min(firstPos,secondPos)) && (positionToMove <= max(firstPos,secondPos)))
		return 1;
	std::vector<ExtendedChar> tmpvect;
	for (int i = firstPos; i < secondPos; i++)
	{
		tmpvect.push_back(data[i]);
	}
	if (positionToMove <= max(firstPos,secondPos))
	{
		data.erase(data.begin() + min(firstPos,secondPos),data.begin() + max(firstPos,secondPos));
		father->text->AddVectorExtendedChar(tmpvect,positionToMove);
	}
	else
	{
		father->text->AddVectorExtendedChar(tmpvect,positionToMove);
		data.erase(data.begin() + min(firstPos,secondPos),data.begin() + max(firstPos,secondPos));
	}
	father->historycontrol->AddState();
	return 1;
}
int Text::GetSize()
{
	return data.size();
}