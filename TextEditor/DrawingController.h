#pragma once
#include "stdafx.h"
class AllWhatYouWantController;
class DrawingController
{
public:
	DrawingController(AllWhatYouWantController* contr);
	~DrawingController(void);
	int PaintAll(POINT* mouseCoord,INT* position); // одновременно и рисует, и позицию ищет, классно же
	int PaintCaret();
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
	int caretPosX;
	int caretPosY;
	HFONT currentFont;
	HDC hdc;
	AllWhatYouWantController* father;
};
