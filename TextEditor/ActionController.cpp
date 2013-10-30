#include "stdafx.h"


ActionController::ActionController(AllWhatYouWantController* contr)
{
	father = contr;
	currentPositionToWrite = 0;
	firstSelectPosition = 0;
	secondSelectPosition = 0;
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
int ActionController::Select(int pos1, int pos2)
{
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
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(father->hWindow->_hwnd, &ps);
	int xcoord =0 , ycoord = 0;
	LPRECT wndRect = new RECT();
	GetClientRect(father->hWindow->_hwnd, wndRect);
    std::vector<ExtendedChar> extchrvector = father->text->data;
	ExtendedChar walker;
	HFONT currentFont;
	SIZE elementSize;
	int i = 0;
	for(i ; i < extchrvector.size();i++)
	{
		walker = extchrvector[i];
		if (walker.bmp != NULL)
		{
			continue;
		}
		GetTextExtentPoint32(hdc,(LPCWSTR)&walker.chr,1, &elementSize);
		if ((pt.x - xcoord < elementSize.cx) && (pt.y - ycoord < elementSize.cy))
		{
			//free(&pt);
			return i;
		}
		if ( (xcoord + elementSize.cx) > wndRect->right - wndRect->left)
		{
			xcoord = 0;
			ycoord = ycoord + elementSize.cy;	
		}
		else
			xcoord+=elementSize.cx;
	}
	//free(&pt);
	return i;
}
int ActionController::ChangeFont()
{
	HDC hdc = GetDC(father->hWindow->_hwnd);
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
	return 1;
}
