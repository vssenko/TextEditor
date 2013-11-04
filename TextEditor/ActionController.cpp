#include "stdafx.h"

ActionController::ActionController(AllWhatYouWantController* contr)
{
	father = contr;
	currentPositionToWrite = 0;
	firstSelectPosition = 0;
	secondSelectPosition = 0;
	isSelected = false;
	isStartedSelect = false;
	isMoveSelected  =false;
	scale = 1;
	currentFont = (HFONT) GetStockObject(ANSI_FIXED_FONT);
	delimiters = std::vector<TCHAR>();
	delimiters.push_back(' ');
	delimiters.push_back(',');
	delimiters.push_back('.');
	delimiters.push_back('!');
	delimiters.push_back('?');
	delimiters.push_back('-');
	delimiters.push_back(':');
}
ActionController::~ActionController(void)
{
}
int ActionController::CharPress(LPARAM lparam, WPARAM wparam)
{
	father->text->AddChar((TCHAR) wparam,currentFont, currentPositionToWrite);
	currentPositionToWrite++;
	return 1;
}
int ActionController::MoveSelected(int pos)
{
	BOOL isNeedToMove = true;
	if ((pos>=min(firstSelectPosition,secondSelectPosition))&&
		(pos<=max(firstSelectPosition,secondSelectPosition)))
		isNeedToMove = false;
	int positionToAdd = std::abs( firstSelectPosition - secondSelectPosition);
	if (isNeedToMove)
		father->text->Move(firstSelectPosition,secondSelectPosition,pos);
	if (isNeedToMove)
	{
		currentPositionToWrite += positionToAdd;
		currentPositionToWrite = min(currentPositionToWrite, father->text->GetSize());
		firstSelectPosition = currentPositionToWrite;
		secondSelectPosition = currentPositionToWrite;
	}
	else
	{
		currentPositionToWrite = max(firstSelectPosition,secondSelectPosition);
		firstSelectPosition = currentPositionToWrite;
		secondSelectPosition = currentPositionToWrite;
	}
	isMoveSelected = false;
	InvalidateRect(father->hWindow->_hwnd,NULL,TRUE);
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
		currentPositionToWrite = pos2;
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
		currentPositionToWrite = pos2;
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
int ActionController::CalculatePosition(int x, int y)//возвращает позицию!
{
	POINT pt = POINT();
	pt.x = x;
	pt.y = y;
	int position = 0;
	std::vector< std::pair<ExtendedChar,POINT> > map = std::vector< std::pair<ExtendedChar,POINT> >();
	CalculateExtendCharCoordinates(&map);
	HDC hdc = GetDC(father->hWindow->_hwnd);
	SIZE vasya = SIZE();
	father->actioncontrol->GetExtendedElementSize(hdc,map.back().first, &vasya);
	int mindy = 10000000, mindx = 10000000;
	int currentdelta;
	for (int i = 0; i< map.size(); i++)
	{
		currentdelta = y - map[i].second.y;
		if (currentdelta>=0)
			mindy = min(mindy, currentdelta);
	}
	for (int i = 0; i< map.size(); i++)
	{
		if (y - map[i].second.y == mindy)
		{
			currentdelta = x - map[i].second.x;
			if (currentdelta >= 0)
				mindx = min(mindx, currentdelta);
		}
	}
	for (int i = 0; i < map.size(); i++)
	{
		if ((y - map[i].second.y == mindy) &&
			(x - map[i].second.x == mindx))
		{
			position = i;
			break;
		}
	}
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
	ZeroMemory(&lf, sizeof(lf));
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
	InvalidateRect(father->hWindow->_hwnd, NULL,TRUE);
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
    std::vector<ExtendedChar> extchrvector;
	father->text->GetData(&extchrvector);
	TEXTMETRIC tm;
	ExtendedChar vasya = ExtendedChar();
	vasya.chr = '\0';
	vasya.font = currentFont;
	extchrvector.push_back(vasya);
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
			if(father->actioncontrol->GetWordSize(hdc,father->text,i,elementSize))
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
		father->actioncontrol->GetExtendedElementSize(hdc,walker,elementSize);
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
		if (father->actioncontrol->IsDelimiter(walker) )
			isSingleWord = true;
		maxLineY = max(maxLineY, elementSize->cy);
	}
	return 1;
}
int ActionController::SelectWord(int pos)
{
	std::vector<ExtendedChar> extchrvector;
	father->text->GetData(&extchrvector);
	BOOL gocha= false;
	int i;
	for ( i = pos; i >= 0; i--)
		if (IsDelimiter(extchrvector[i]))
		{
			gocha = true;
			firstSelectPosition = i + 1;
			break;
		}
	if (!gocha)
		firstSelectPosition = 0;
		for ( i = pos; i < extchrvector.size(); i++)
		if (IsDelimiter(extchrvector[i]))
			break;
	secondSelectPosition = i;
	InvalidateRect(father->hWindow->_hwnd, NULL,TRUE);
	return 1;
}
int ActionController::GetExtendedElementSize(HDC hdc, ExtendedChar chr, SIZE* size)
{
	if (chr.bmp != NULL)
	{
		BITMAP realbmp;
		GetObject(chr.bmp, sizeof(BITMAP),(LPVOID) &realbmp); 
		size->cx = realbmp.bmWidth;
		size->cy = realbmp.bmHeight;
		return 1;
	}
	SIZE sz;
	HFONT font = (HFONT) GetCurrentObject(hdc,OBJ_FONT);
	if (font != chr.font)
	{
		TEXTMETRIC tm;
		HFONT prevFont = (HFONT) SelectObject(hdc,chr.font);
		GetTextMetrics(hdc,&tm);
		GetTextExtentPoint32(hdc,(LPCWSTR)&chr.chr,1, &sz);
		sz.cx += tm.tmInternalLeading;
	}
	else
	{
		TEXTMETRIC tm;
		GetTextMetrics(hdc,&tm);
		GetTextExtentPoint32(hdc,(LPCWSTR)&chr.chr,1, &sz);
		sz.cx += tm.tmInternalLeading;
	}
	size->cx = sz.cx;
	size->cy = sz.cy;
	return 1;
}
int ActionController::GetWordSize(HDC hdc, Text* text, int currentpos, SIZE* size)//меняет size, возвращает, были ли дальше разделители
{
	SIZE* currentSize = new SIZE();
	size->cx = 0;
	size-> cy = 0;
	ExtendedChar walker;
	BOOL isDelimiter = false;
	std::vector<ExtendedChar> extchrvectr;
	father->text->GetData(&extchrvectr);
	for (int i = currentpos; i< extchrvectr.size(); i++)
	{
		walker = extchrvectr[i];
		if (IsDelimiter(walker))
		{
			isDelimiter = true;
			break;
		}
		if (walker.bmp != NULL)
			isDelimiter = true;
		if (isDelimiter)
			break;
		GetExtendedElementSize(hdc,walker,currentSize);
		size->cx += currentSize->cx;
		size->cy = max(size->cy,currentSize->cy);
	}
	return 1;
}
BOOL ActionController::IsDelimiter(ExtendedChar chr)
{
	BOOL isDelimiter = false;
	for (int j=0; j< delimiters.size(); j++)
		if ( chr.chr == delimiters[j])
		{
			isDelimiter = true;
			break;
		}
	return isDelimiter;
}
int ActionController::DeleteSelected()
{
	if (firstSelectPosition!= secondSelectPosition)
		father->text->DeleteSymbol(firstSelectPosition,secondSelectPosition);
	currentPositionToWrite = min(firstSelectPosition,secondSelectPosition);
	return 1;
}