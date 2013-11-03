#include "stdafx.h"
DrawingController::DrawingController(AllWhatYouWantController* contr)
{
	father = contr;
	delimiters = std::vector<TCHAR>();
	delimiters.push_back(' ');
	delimiters.push_back(',');
	delimiters.push_back('.');
	delimiters.push_back('!');
	delimiters.push_back('?');
	delimiters.push_back('-');
	delimiters.push_back(':');
	caretPosX = 0;
	caretPosY = 0;
}
DrawingController::~DrawingController(void)
{
}
int DrawingController::DrawBitmap(HBITMAP hBitmap) 
{ 
	 BITMAP bm; 
	 HDC hdcMem; 
	 DWORD dwSize; 
	 POINT ptSize, ptOrg; 
	 hdcMem = CreateCompatibleDC(hdc); 
	 SelectObject(hdcMem, hBitmap); 
	 SetMapMode(hdcMem, GetMapMode(hdc)); 
	 GetObject(hBitmap, sizeof(BITMAP),(LPVOID) &bm); 
	 ptSize.x = bm.bmWidth; 
	 ptSize.y = bm.bmHeight; 
	 DPtoLP(hdc, &ptSize, 1); 
	 ptOrg.x = 0; 
	 ptOrg.y = 0; 
	 DPtoLP(hdcMem, &ptOrg, 1); 
	 BitBlt(
	hdc, xcoord, ycoord, ptSize.x, ptSize.y, 
	hdcMem, ptOrg.x, ptOrg.y, SRCCOPY 
	); 
	 DeleteDC(hdcMem); 
	 return 1337;
}
int DrawingController::PaintAll()
{
	currentFont = 0;
	BOOL isCaretLocated = false, isSingleWord = false;
	isSelected = false;
	xcoord = 0;
	ycoord = 0;
	int maxLineY = 0;
	PAINTSTRUCT ps;
	std::vector<std::pair<ExtendedChar,POINT>> map;
	father->actioncontrol->CalculateExtendCharCoordinates(&map);
	map.pop_back(); // удаляем последний "нулевой" символ
	RECT* wndRect = new RECT();
	GetClientRect(father->hWindow->_hwnd, wndRect);
	std::pair<ExtendedChar,POINT> walker;
	SIZE* elementSize = new SIZE();
	hdc = BeginPaint(father->hWindow->_hwnd, &ps);
	for(int i= 0; i< map.size();i++)
	{
		if (i == father->actioncontrol->currentPositionToWrite)
		{
			isCaretLocated = true;
			caretPosX = xcoord;
			caretPosY = ycoord;
			PaintCaret();
		}
		if (i == min(father->actioncontrol->firstSelectPosition,
			father->actioncontrol->secondSelectPosition))
		{
			isSelected = TRUE;
			SetBkColor(hdc, RGB(0,0,255));
		}
		if (i == max(father->actioncontrol->firstSelectPosition,
			father->actioncontrol->secondSelectPosition))
		{
			isSelected = FALSE;
			SetBkColor(hdc, RGB(255,255,255));
		}
		walker = map[i];
		SetFont(walker.first);
		xcoord = walker.second.x;
		ycoord = walker.second.y;
		DrawExtendedChar(walker.first);
	}
	if (!isCaretLocated)
	{
		if (map.size() != 0)
		{
			father->drawingcontrol->GetExtendedElementSize(hdc,map.back().first, elementSize);
			caretPosX = xcoord + elementSize->cx;
			caretPosY = ycoord;
		}
		PaintCaret();
	}
	EndPaint(father->hWindow->_hwnd, &ps);
	return 1;
}
int DrawingController::DrawExtendedChar(ExtendedChar chr)
{
	if ( chr.bmp != NULL)
	{
		DrawBitmap(chr.bmp);
		return 1;
	}
	else
	{
		SetFont(chr);
		::TextOut(hdc, xcoord, ycoord, (LPCWSTR) &chr.chr, 1 );
		return 1;
	}
}
int DrawingController::PaintCaret()
{
	DestroyCaret();
	TEXTMETRIC tm;
	SelectObject(hdc, father->actioncontrol->currentFont);
	GetTextMetrics(hdc,&tm);
	CreateCaret(father->hWindow->_hwnd,NULL,1, tm.tmHeight);
	SetCaretPos(caretPosX, caretPosY);
	ShowCaret(father->hWindow->_hwnd);
	return 1;
}
int DrawingController::GetExtendedElementSize(HDC hdc, ExtendedChar chr, SIZE* size)
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
BOOL DrawingController::GetWordSize(HDC hdc, Text* text, int currentpos, SIZE* size)//меняет size, возвращает, были ли дальше разделители
{
	SIZE* currentSize = new SIZE();
	size->cx = 0;
	size-> cy = 0;
	ExtendedChar walker;
	BOOL isDelimiter = false;
	for (int i = currentpos; i< text->data.size(); i++)
	{
		walker = text->data[i];
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
	return true;
}
BOOL DrawingController::IsDelimiter(ExtendedChar chr)
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
int DrawingController::SetFont(ExtendedChar chr)
{
	if (currentFont != chr.font)
	{
		SelectObject(hdc,chr.font);
		currentFont = chr.font;
	}
	return 1;
}