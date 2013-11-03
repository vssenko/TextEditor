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
int DrawingController::PaintAll(POINT* mouseCoord, INT* position)//не только рисует, но и позицию ищет
{
	BOOL isCaretLocated = false, isSelected = false, isSingleWord = false,isNeedToFindPosition = false;
	if (mouseCoord != NULL && position != NULL)
		isNeedToFindPosition = true;
	xcoord = 0;
	ycoord = 0;
	int maxLineY = 0;
	PAINTSTRUCT ps;
	hdc = BeginPaint(father->hWindow->_hwnd, &ps);
	SetTextAlign(hdc, TA_LEFT);
	LPRECT wndRect = new RECT();
	currentFont = NULL;
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
		walker = extchrvector[i];
		SetFont(walker);
		if (isSingleWord)
		{
			if(GetWordSize(hdc,father->text,i,elementSize))
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
		GetExtendedElementSize(hdc,walker,elementSize);
		if ((xcoord + elementSize->cx) > wndRect->right - wndRect->left)
		{
			GetTextMetrics(hdc,&tm);
			xcoord = tm.tmInternalLeading;
			ycoord = ycoord + maxLineY;	
			maxLineY = 0;
		}
		if (isNeedToFindPosition)
		{
			POINT vasya = POINT();
			vasya.x = xcoord;
			vasya.y = ycoord;
			points.push_back(vasya);
		}
		else
			DrawExtendedChar(walker);
		xcoord = xcoord + elementSize->cx;
		if (IsDelimiter(walker) )
			isSingleWord = true;
		maxLineY = max(maxLineY, elementSize->cy);
	}
	if (!isCaretLocated)
	{
		caretPosX = xcoord;
		caretPosY = ycoord;
		PaintCaret();
	}
	if (isSelected)
	{
		isSelected = FALSE;
		SetBkColor(hdc, RGB(255,255,255));
	}
	if (isNeedToFindPosition)
	{
		int mindy = 100000,  mindx = 100000;
		for(int i = 0; i< points.size(); i++)
		{
			if ((points[i].y - mouseCoord->x < mindy) 
				&&(points[i].y - mouseCoord->x>=0))
			{
				mindy = points[i].y - mouseCoord->x;
			}
		}
		int pos = points.size();
		for(int i = 0; i< points.size(); i++)
		{
			if( (points[i].y - mouseCoord->y == mindy)
				&&(points[i].x -mouseCoord->x <mindx)
				&&(points[i].x -mouseCoord->x >= 0))
			{
				pos = i;
				mindx = points[i].x -mouseCoord->x;
			}
		}
		*position = pos;
	}
	EndPaint(father->hWindow->_hwnd, &ps);
	delete elementSize;
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
	if (currentFont != chr.font)
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
		TEXTMETRIC tm; 
		GetTextMetrics(hdc, &tm);
		//int i =0;
		//i++;
	}
	return 1;
}