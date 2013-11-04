#pragma once
#include "stdafx.h"
class AllWhatYouWantController;
class Text
{
public:
	Text(AllWhatYouWantController* contr);
	~Text(void);
	int DeleteSymbol(int pos);
	int DeleteSymbol(int pos1, int pos2);
	int AddBitmap(HBITMAP bitmap, int pos);
	int AddChar(TCHAR chr, HFONT font, int pos);
	int AddVectorExtendedChar(std::vector<ExtendedChar> vctr, int pos);
	int Move(int firstPos,int secondPos, int positionToMove);
	int  GetData(std::vector<ExtendedChar>* vectr);
	int GetSize();
private:
	AllWhatYouWantController* father;
	std::vector<ExtendedChar> data;
};

