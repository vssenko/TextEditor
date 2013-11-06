#pragma once
#include "stdafx.h"
class AllWhatYouWantController;
class Text: public BaseController
{
public:
	Text(AllWhatYouWantController* contr);
	~Text(void);
	int DeleteSymbol(int pos,BOOL toHistory);
	int DeleteSymbol(int pos1, int pos2,BOOL toHistory);
	int AddBitmap(HBITMAP bitmap, int pos,BOOL toHistory);
	int AddChar(TCHAR chr, HFONT font, int pos,BOOL toHistory);
	int AddVectorExtendedChar(std::vector<ExtendedChar> vctr, int pos,BOOL toHistory);
	int Move(int firstPos,int secondPos, int positionToMove,BOOL toHistory);
	int SetNewData(std::vector<ExtendedChar> chrvctr,BOOL toHistory);
	int  GetData(std::vector<ExtendedChar>* vectr);
	int GetSize();
	int ChangeFont(int pos1,int pos2, HFONT font);
private:
	std::vector<ExtendedChar> data;
};

