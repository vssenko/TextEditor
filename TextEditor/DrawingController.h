
#pragma once

#include "stdafx.h"
class AllWhatYouWantController;
class DrawingController
{
public:
	DrawingController(AllWhatYouWantController* contr);
	~DrawingController(void);
	int PaintAll();
	int SetCaret(int x, int y);
	int GetExtendedElementSize(HDC hdc, ExtendedChar chr, SIZE* size);
	BOOL GetWordSize(HDC hdc, Text* text, int currentpos, SIZE* size);
	BOOL IsDelimiter(ExtendedChar chr);
private:
	int DrawBitmap(HBITMAP hBitmap);
	int DrawExtendedChar(ExtendedChar chr);
	int SetFont(ExtendedChar chr);
	std::vector<TCHAR> delimiters;
	int xcoord;
	int ycoord;
	HFONT currentFont;
	HDC hdc;
	AllWhatYouWantController* father;
};
