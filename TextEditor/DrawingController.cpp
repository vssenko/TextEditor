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
	BOOL isCaretLocated = false, isSelected = false, isSingleWord = false;
	xcoord = 0;
	ycoord = 0;
	int maxLineY = 0;
	PAINTSTRUCT ps;
	hdc = BeginPaint(father->hWindow->_hwnd, &ps);
	LPRECT wndRect = new RECT();
	currentFont = NULL;
	GetClientRect(father->hWindow->_hwnd, wndRect);
    std::vector<ExtendedChar> extchrvector = father->text->data;
	ExtendedChar walker;	
	SIZE* elementSize = new SIZE();
	for(int i= 0; i< extchrvector.size();i++)
	{
		if (i == father->actioncontrol->currentPositionToWrite)
		{
			isCaretLocated = true;
			SetCaret(xcoord,ycoord);
		}
		if (i == father->actioncontrol->firstSelectPosition)
		{
			isSelected = TRUE;
			SetBkColor(hdc, RGB(0,0,255));
		}
		if (i == father->actioncontrol->secondSelectPosition)
		{
			isSelected = FALSE;
			SetBkColor(hdc, RGB(255,255,255));
		}
		walker = extchrvector[i];
		SetFont(walker);
		if (isSingleWord)
		{
			if(GetWordSize(hdc,father->text,i,elementSize))
			{
				if (xcoord + elementSize->cx > wndRect->right - wndRect->left)
				{
					xcoord = 0;
					ycoord += maxLineY;
				}
			}
			isSingleWord = false;
		}
		GetExtendedElementSize(hdc,walker,elementSize);
		if ((xcoord + elementSize->cx) > wndRect->right - wndRect->left)
		{
			xcoord = 0;
			ycoord = ycoord + maxLineY;	
		}
		DrawExtendedChar(walker);
		xcoord = xcoord + elementSize->cx + 1;// ПЛЮС ОДИНОДИН)))0000
		if (IsDelimiter(walker) )
			isSingleWord = true;
		maxLineY = max(maxLineY, elementSize->cy);
	}
	if (!isCaretLocated)
		SetCaret(xcoord,ycoord);
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
		::TextOut(hdc,xcoord,ycoord,(LPCWSTR)&chr.chr,1);
		return 1;
	}
}
int DrawingController::SetCaret(int x, int y)
{
	DestroyCaret();
	CreateCaret(father->hWindow->_hwnd,NULL,1,16);
	SetCaretPos(x, y);
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
	if (currentFont != chr.font)
	{
		HFONT prevFont = (HFONT) SelectObject(hdc,chr.font);
		GetTextExtentPoint32(hdc,(LPCWSTR)&chr.chr,1, &sz);
		SelectObject(hdc,prevFont);
	}
	else
	{
		GetTextExtentPoint32(hdc,(LPCWSTR)&chr.chr,1, &sz);
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