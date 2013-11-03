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
	father->drawingcontrol->PaintAll();
	return 1;
}
int ActionController::SetFocus()
{
	father->drawingcontrol->PaintCaret();
	return 1;
}
int ActionController::CalculateExtendCharCoordinates(std::vector<std::pair<ExtendedChar,POINT>>* map)
{
	BOOL isCaretLocated = false, isSelected = false, isSingleWord = false,isNeedToFindPosition = false;
	int xcoord = 0;
	int ycoord = 0;
	int maxLineY = 0;
	POINT point;
	HDC hdc = GetDC(father->hWindow->_hwnd);
	SetTextAlign(hdc, TA_LEFT);
	LPRECT wndRect = new RECT();
	HFONT font = NULL;
	GetClientRect(father->hWindow->_hwnd, wndRect);
    std::vector<ExtendedChar> extchrvector = father->text->data;
	TEXTMETRIC tm;
	if (extchrvector.size() > 0)
	{
		SelectObject(hdc,extchrvector[0].font);
		GetTextMetrics(hdc,&tm);
		xcoord = tm.tmInternalLeading;
	}
	ExtendedChar walker;	
	SIZE* elementSize = new SIZE();
	std::vector<POINT> points;
	for(int i= 0; i< extchrvector.size();i++)
	{
		walker = extchrvector[i];
		if (font != walker.font)
		{
			SelectObject(hdc,walker.font);
			font = walker.font;
		}
		if (isSingleWord)
		{
			if(father->drawingcontrol->GetWordSize(hdc,father->text,i,elementSize))
			{
				if (xcoord + elementSize->cx > wndRect->right - wndRect->left)
				{
					GetTextMetrics(hdc,&tm);
					xcoord = tm.tmInternalLeading;
					ycoord += maxLineY;
					maxLineY = 0;
				}
			}
			isSingleWord = false;
		}
		father->drawingcontrol->GetExtendedElementSize(hdc,walker,elementSize);
		if ((xcoord + elementSize->cx) > wndRect->right - wndRect->left)
		{
			GetTextMetrics(hdc,&tm);
			xcoord = tm.tmInternalLeading;
			ycoord = ycoord + maxLineY;	
			maxLineY = 0;
		}
		point = POINT();
		point.x = xcoord;
		point.y = ycoord;
		map->push_back(std::make_pair(walker,point));
		xcoord = xcoord + elementSize->cx;
		if (father->drawingcontrol->IsDelimiter(walker) )
			isSingleWord = true;
		maxLineY = max(maxLineY, elementSize->cy);
	}
	return 1;
}