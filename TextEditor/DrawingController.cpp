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
	BOOL isCaretLocated = false;
	xcoord = 0;
	ycoord = 0;
	PAINTSTRUCT ps;
	hdc = BeginPaint(father->hWindow->_hwnd, &ps);
	LPRECT wndRect = new RECT();
	HFONT currentFont = NULL;
	GetClientRect(father->hWindow->_hwnd, wndRect);
    std::vector<ExtendedChar> extchrvector = father->text->data;
	ExtendedChar walker;	SIZE elementSize;
	BOOL isSelected = false;
	for(int i= 0; i< extchrvector.size();i++)
	{
		if (i == father->actioncontrol->currentPositionToWrite)
		{
			isCaretLocated = true;
			father->actioncontrol->SetCaret(xcoord,ycoord);
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
		DrawExtendedChar(walker);
		GetTextExtentPoint32(hdc,(LPCWSTR)&walker.chr,1, &elementSize);
		if ((xcoord + elementSize.cx) > wndRect->right - wndRect->left)
		{
			xcoord = 0;
			ycoord = ycoord + elementSize.cy;	
		}
		::TextOut(hdc,xcoord,ycoord,(LPCWSTR)&walker.chr,1);
		xcoord = xcoord + elementSize.cx;
	}
	if (!isCaretLocated)
		father->actioncontrol->SetCaret(xcoord,ycoord);
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

	return 1;
}
int DrawingController::GetExtendedElementSize(ExtendedChar chr, SIZE* size)
{
	if (chr.bmp != NULL)
	{

		return 1;
	}
	return 1;
}