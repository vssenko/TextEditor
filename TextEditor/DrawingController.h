#include "stdafx.h"
#pragma once
class DrawingController
{
public:
	DrawingController(AllWhatYouWantController* contr);
	~DrawingController(void);
	int DrawBitmap(HDC hdc,HBITMAP hBitmap, int xStart, int yStart);
	int PaintAll();
private:
	AllWhatYouWantController* father;
};

