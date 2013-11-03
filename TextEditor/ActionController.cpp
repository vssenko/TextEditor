#include "stdafx.h"


ActionController::ActionController(AllWhatYouWantController* contr)
{
	father = contr;
	currentPositionToWrite = 0;
	firstSelectPosition = 0;
	secondSelectPosition = 0;
	isStartedSelect = false;
	scale = 1;
	currentFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
}
ActionController::~ActionController(void)
{
}
int ActionController::CharPress(LPARAM lparam, WPARAM wparam)
{
	ExtendedChar charrr =  ExtendedChar();
	charrr.font = currentFont;
	charrr.chr = (TCHAR) wparam;
	father->text->data.insert(father->text->data.begin() + currentPositionToWrite, charrr);
	currentPositionToWrite++;
	return 1;
}
int ActionController::MoveSelected()
{
	return 1;
}
int ActionController::Select(int pos1, int pos2)// если позиция меньше нуля, то не трогаем ее
{
	int cfp = firstSelectPosition;
	int csp = secondSelectPosition;
	if ( (pos1>=0) && (pos2>=0))
	{
		firstSelectPosition = pos1;
		secondSelectPosition = pos2;
	}
	else
	if (pos1>=0)
	{
		firstSelectPosition = pos1;
	}
	else
	if (pos2>=0)
	{
		secondSelectPosition = pos2;
	}
	if(firstSelectPosition !=cfp || secondSelectPosition != csp)
		InvalidateRect(father->hWindow->_hwnd,NULL,TRUE);
	return 1;
}
int ActionController::IncreaseScale()
{
	scale = scale +0.1;
	if (scale > 2)
	{
		scale = 2;
	}
	return 1;
}
int ActionController::DecreaseScale()
{
	scale = scale - 0.1;
	if (scale < 0.5)
	{
		scale = 0.5;
	}
	return 1;
}
int ActionController::SpecialKeyPressed(WPARAM key)
{
	return 1;
}
int ActionController::CalculatePosition(int x, int y)
{
	POINT pt = POINT();
	pt.x = x;
	pt.y = y;
	int position = 0;
	father->drawingcontrol->PaintAll(&pt,&position);
	return position;
}
int ActionController::ChangeFont()
{
	CHOOSEFONT cf;            // common dialog box structure
	static LOGFONT lf;        // logical font structure
	static DWORD rgbCurrent;  // current text color
	HFONT hfont, hfontPrev;
	DWORD rgbPrev;
	// Initialize CHOOSEFONT
	ZeroMemory(&cf, sizeof(cf));
	cf.lStructSize = sizeof (cf);
	cf.hwndOwner = father->hWindow->_hwnd;
	cf.lpLogFont = &lf;
	cf.rgbColors = rgbCurrent;
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
	if (ChooseFont(&cf)==TRUE)
	{
		hfont = CreateFontIndirect(cf.lpLogFont);
		currentFont = hfont;
	}
	father->drawingcontrol->PaintAll(NULL,NULL);
	return 1;
}
int ActionController::SetFocus()
{
	father->drawingcontrol->PaintCaret();
	return 1;
}