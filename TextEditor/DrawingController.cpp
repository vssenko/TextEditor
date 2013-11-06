#include "stdafx.h"
DrawingController::DrawingController(AllWhatYouWantController* contr)
{
	father = contr;
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
	 DWORD params;
	 if (isSelected)
		 params = DSTINVERT;
	 else
		 params = SRCCOPY;
	 BitBlt(
		 hdc, xcoord, ycoord, ptSize.x * father->scalingcontrol->scale, ptSize.y * father->scalingcontrol->scale, 
	hdcMem, ptOrg.x, ptOrg.y, params 
	); 
	//StretchBlt(hdc,xcoord,ycoord, ptSize.x * scaleRatio, ptSize.y * scaleRatio, hdcMem, 0, 0, image->GetWidth(), image->GetHeight(), SRCCOPY);
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
	maxLineY = 0;
	PAINTSTRUCT ps;
	std::vector<std::pair<ExtendedChar,POINT>> map;
	father->actioncontrol->CalculateExtendCharCoordinates(&map);
	int xlastCoord , ylastCoord;//координаты конца текста
	xlastCoord = map.back().second.x;
	ylastCoord = map.back().second.y;
	map.pop_back(); // удаляем последний "нулевой" символ
	RECT* wndRect = new RECT();
	GetClientRect(father->hWindow->_hwnd, wndRect);
	std::pair<ExtendedChar,POINT> walker;
	SIZE* elementSize = new SIZE();
	hdc = BeginPaint(father->hWindow->_hwnd, &ps);
	SetTextAlign(hdc, TA_LEFT | TA_TOP);
	for(int i= 0; i< map.size();i++)
	{
		walker = map[i];
		SetFont(walker.first);
		xcoord = walker.second.x;
		ycoord = walker.second.y;
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
		DrawExtendedChar(walker.first);
	}
	if (!isCaretLocated)
	{
			caretPosX = xlastCoord;
			caretPosY = ylastCoord;
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
		switch (chr.chr)
		{
		case '\t':
			::TextOut(hdc,xcoord, ycoord,L"    ",4);
			break;
		default:
			::TextOut(hdc, xcoord, ycoord, (LPCWSTR) &chr.chr, 1 );
			break;
		}
		return 1;
	}
}
int DrawingController::PaintCaret()
{
	DestroyCaret();
	TEXTMETRIC tm;
	HFONT oldFont = (HFONT)SelectObject(hdc,
		father->scalingcontrol->ScaledFont(father->actioncontrol->currentFont));
	GetTextMetrics(hdc,&tm);
	CreateCaret(father->hWindow->_hwnd,NULL,1, tm.tmHeight);
	SetCaretPos(caretPosX, caretPosY);
	ShowCaret(father->hWindow->_hwnd);
	SelectObject(hdc, oldFont);
	return 1;
}
int DrawingController::SetFont(ExtendedChar chr)
{
	if (currentFont != chr.font)
	{
		SelectObject(hdc,father->scalingcontrol->ScaledFont(chr.font));
		currentFont = chr.font;
	}
	return 1;
}