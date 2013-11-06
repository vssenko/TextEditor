
#include "stdafx.h"
Text::Text(AllWhatYouWantController* contr)
{
	data = std::vector<ExtendedChar>();
	father = contr;
}
Text::~Text(void)
{
}
int Text::AddChar(TCHAR chr, HFONT font, int pos,BOOL toHistory)
{
	if (toHistory)
		father->historycontrol->AddState();
	ExtendedChar extchr = ExtendedChar();
	extchr.chr = chr;
	extchr.font = font;
	data.insert(data.begin() + pos, extchr);
	return 1;
}
int Text::AddBitmap(HBITMAP bitmap, int pos,BOOL toHistory)
{
	if (toHistory)
		father->historycontrol->AddState();
	if (bitmap ==NULL)
		return 0;
	ExtendedChar bitmpchar =ExtendedChar();
	bitmpchar.bmp = bitmap;
	this->data.insert(data.begin() + pos, bitmpchar);
	return 1;
}
int Text::DeleteSymbol(int pos,BOOL toHistory)
{
	if (toHistory)
		father->historycontrol->AddState();
	data.erase(data.begin() + pos);
	
	return 1;
}
int Text::DeleteSymbol(int pos1,int pos2,BOOL toHistory)
{
	if (toHistory)
		father->historycontrol->AddState();
	data.erase(data.begin() + min(pos1,pos2),data.begin() + max(pos1,pos2));
	
	return 1;
}
int Text::GetData(std::vector<ExtendedChar>* vectr)
{
	*vectr = data;
	return 1;
}
int Text::AddVectorExtendedChar(std::vector<ExtendedChar> vctr, int pos,BOOL toHistory)
{
	if (toHistory)
		father->historycontrol->AddState();
	for(int i = 0; i < vctr.size(); i++)
	{
		ExtendedChar chr = vctr[i];
		data.insert(data.begin() + pos, chr);
		pos++;
	}
	return 1;
}
int Text::Move(int firstPos,int secondPos, int positionToMove,BOOL toHistory)
{
	if (toHistory)
	father->historycontrol->AddState();
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
		father->text->AddVectorExtendedChar(tmpvect,positionToMove,FALSE);
	}
	else
	{
		father->text->AddVectorExtendedChar(tmpvect,positionToMove,FALSE);
		data.erase(data.begin() + min(firstPos,secondPos),data.begin() + max(firstPos,secondPos));
	}
	return 1;
}
int Text::GetSize()
{
	return data.size();
}
int Text::SetNewData(std::vector<ExtendedChar> chrvctr,BOOL toHistory)
{
	data = chrvctr;
	if (toHistory)
		father->historycontrol->ClearHistory();
	father->actioncontrol->currentPositionToWrite = data.size();
	father->actioncontrol->firstSelectPosition = data.size();
	father->actioncontrol->secondSelectPosition = data.size();
	InvalidateRect(father->hWindow->_hwnd, NULL,TRUE);
	return 1;
}
int Text::ChangeFont(int pos1,int pos2, HFONT font)
{
	for (int i = pos1; i< pos2; i++)
	{
		data[i].font = font;
	}
	return 1;
}