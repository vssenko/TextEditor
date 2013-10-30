
#pragma once

#include "stdafx.h"
class AllWhatYouWantController;
class DrawingController
{
public:
	DrawingController(AllWhatYouWantController* contr);
	~DrawingController(void);
	int PaintAll();
private:
	int DrawBitmap(HBITMAP hBitmap);
	int DrawExtendedChar(ExtendedChar chr);
	int GetExtendedElementSize(ExtendedChar chr, SIZE* size);
	std::vector<TCHAR> delimiters;
	int xcoord;
	int ycoord;
	HDC hdc;
	AllWhatYouWantController* father;
};
