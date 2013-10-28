#include "stdafx.h"



DrawingController::DrawingController(AllWhatYouWantController* contr)
{
	father = contr;
}
DrawingController::~DrawingController(void)
{
}
int DrawingController::DrawBitmap(HDC hdc,HBITMAP hBitmap, int xStart, int yStart) 
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
	hdc, xStart, yStart, ptSize.x, ptSize.y, 
	hdcMem, ptOrg.x, ptOrg.y, SRCCOPY 
	); 
	 DeleteDC(hdcMem); 
	 return 1337;
}
int DrawingController::PaintAll()
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWindow->_hwnd, &ps);
    int xcoord=0, //ииииикс
		ycoord=0; ///иииигрик
	LPRECT wndRect = new RECT();
	SelectObject(hdc, NULL);
	GetClientRect(hWindow->_hwnd, wndRect);
    std::vector<ExtendedChar> extchrvector = text->GetStringText();
	ExtendedChar walker;
	bool isSelected = mouseDownPosition != mouseUpPosition;
	HFONT currentFont;
	SIZE elementSize;
	for(int i= 0; i<= extchrvector.size();i++)
	{
		if (i == currentPositionToWrite)
		{
			DestroyCaret();
			CreateCaret(hWindow->_hwnd,NULL,1,16);
			SetCaretPos(xcoord,ycoord);
			ShowCaret(hWindow->_hwnd);
		}
		if (isSelected && i == mouseDownPosition)
		{
			SetBkColor(hdc, RGB(0,0,255));
		}
		if (isSelected && i == mouseUpPosition)
		{
			SetBkColor(hdc, RGB(255,255,255));
		}
		if (i == extchrvector.size())
			break;
		walker = extchrvector[i];
		if (walker.ifImageThenImageIndex != -1)
		{
			BITMAP bm;
			GetObject(text->GetBitmapArray()[walker.ifImageThenImageIndex], sizeof(BITMAP),(LPVOID) &bm); 
			if (bm.bmWidth + xcoord > wndRect->right - wndRect->left)
			{
				xcoord = 0;
				ycoord = 
			}
			DrawBitmap(hdc, 
				text->GetBitmapArray()[walker.ifImageThenImageIndex],
				xcoord,ycoord);
			
			xcoord += bm.bmWidth;
			ycoord += bm.bmHeight;
			continue;
		}
		GetTextExtentPoint32(hdc,(LPCWSTR)&walker.chr,1, &elementSize);
		if ( (xcoord + elementSize.cx) > wndRect->right - wndRect->left)
		{
			xcoord = 0;
			ycoord = ycoord + elementSize.cy;	
		}
		::TextOut(hdc,xcoord,ycoord,(LPCWSTR)&walker.chr,1);
		xcoord = xcoord + elementSize.cx;
	}
	EndPaint(hWindow->_hwnd, &ps);
	return 1;
}