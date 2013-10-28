#include "stdafx.h"


ActionController::ActionController(AllWhatYouWantController* contr)
{
	father = contr;
	currentPositionToWrite = 0;
	firstSelectPosition = 0;
	secondSelectPosition = 0;
}

ActionController::~ActionController(void)
{
}
int ActionController::CharPress(TCHAR chr)
{
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
	return 1;
}
int ActionController::DecreaseScale()
{
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
    std::vector<ExtendedChar> extchrvector = father->text->GetStringText();
	ExtendedChar walker;
	HFONT currentFont;
	SIZE elementSize;
	int i = 0;
	for(i ; i < extchrvector.size();i++)
	{
		walker = extchrvector[i];
		if (walker.ifImageThenImageIndex != -1)
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
	return 1;
}