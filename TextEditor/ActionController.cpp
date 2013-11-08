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
	charRecieved = true;
	uClipboardFormat = RegisterClipboardFormat(L"VASYA");
	currentFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	delimiters = std::vector<TCHAR>();
	delimiters.push_back(' ');
	delimiters.push_back(',');
	delimiters.push_back('.');
	delimiters.push_back('!');
	delimiters.push_back('?');
	delimiters.push_back('-');
	delimiters.push_back(':');
	delimiters.push_back('\t');
}
ActionController::~ActionController(void)
{
}
int ActionController::CharPress(LPARAM lparam, WPARAM wparam)
{
	if (charRecieved)
	{
		switch (wparam)
		{
		case KEY_CTRL_Z:
			father->historycontrol->Repeal();
			break;
		case KEY_CTRL_X:
			CopyToClipboad(true);
			break;
		case KEY_CTRL_C:
			CopyToClipboad(false);
			break;
		case KEY_CTRL_V:
			PasteFromClipboad();
			break;
		case KEY_CTRL_Y:
			father->historycontrol->RepealOfRepeal();
			break;
		case VK_BACK:
			if(firstSelectPosition != secondSelectPosition)
				DeleteSelected();
			else
			{
				currentPositionToWrite = max(currentPositionToWrite - 1, 0);
				father->text->DeleteSymbol(currentPositionToWrite, TRUE);
				firstSelectPosition = currentPositionToWrite;
				secondSelectPosition = currentPositionToWrite;
			}
			InvalidateRect(father->hWindow->_hwnd, NULL,TRUE);
			break;
		default:
			if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000) )//не нажат контрл
			{
				DeleteSelected();
				father->text->AddChar((TCHAR) wparam,currentFont, currentPositionToWrite,TRUE);
				currentPositionToWrite++;
				InvalidateRect(father->hWindow->_hwnd, NULL,TRUE);
			}
		}
	}
	else
		charRecieved = true; // следующий будет ожидаться char
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
		father->text->Move(firstSelectPosition,secondSelectPosition,pos,TRUE);
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
int ActionController::SpecialKeyPressed(WPARAM key)
{
	return 1;
}
int ActionController::CalculatePosition(int x, int y)///*возвращает позицию!*////
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
	if (firstSelectPosition != secondSelectPosition)
		father->text->ChangeFont(min(firstSelectPosition,secondSelectPosition),
								 max(firstSelectPosition,secondSelectPosition),currentFont);
	InvalidateRect(father->hWindow->_hwnd, NULL, TRUE);
	return 1;
}
int ActionController::SetFocus()
{
	//InvalidateRect(father->hWindow->_hwnd, NULL,TRUE);
	return 1;
}
int ActionController::CalculateExtendCharCoordinates(std::vector<std::pair<ExtendedChar,POINT>>* map)
{
	BOOL isCaretLocated = false, isSelected = false, isSingleWord = false,isNeedToFindPosition = false;
	int xcoord = 0;
	int ycoord = 0;
	int maxLineY = 0;
	POINT point;
	ABC abc;
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
	ExtendedChar walker;
	SIZE* elementSize = new SIZE();
	std::vector<POINT> points;
	BOOL isOffsetLeft = false;
	for(int i= 0; i< extchrvector.size();i++)
	{
		walker = extchrvector[i];
		if (font != walker.font)
		{
			SelectObject(hdc,father->scalingcontrol->ScaledFont(walker.font));
			font = walker.font;
		}
		if (walker.bmp == NULL)
		{
			GetCharABCWidths(hdc, extchrvector[i].chr,extchrvector[i].chr,&abc);
			isOffsetLeft = (abc.abcA < 0);	
			if (isOffsetLeft)
				xcoord -= abc.abcA;
			if (isSingleWord)
			{
				if(father->actioncontrol->GetWordSize(hdc,father->text,i,elementSize))
				{
					if (xcoord + elementSize->cx > wndRect->right - wndRect->left)
					{
						if (isOffsetLeft)
							xcoord = -abc.abcA;
						else
							xcoord = 0;
						ycoord += maxLineY;
						maxLineY = 0;
					}
				}
				isSingleWord = false;
			}
		}
		father->actioncontrol->GetExtendedElementSize(hdc,walker,elementSize);
		if (((xcoord + elementSize->cx) > wndRect->right - wndRect->left) || walker.chr == '\r')
		{
			if (isOffsetLeft)
				xcoord = -abc.abcA;
			else
				xcoord = 0;
			ycoord = ycoord + maxLineY;	
			maxLineY = 0;
		}
		point = POINT();
		point.x = xcoord;
		point.y = ycoord;
		map->push_back(std::make_pair(walker,point));
		xcoord = xcoord + elementSize->cx +1;
		if (isOffsetLeft)
			xcoord = xcoord + abc.abcA; //прибавляем отрицат. значение
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
		size->cx = realbmp.bmWidth * father->scalingcontrol->scale;
		size->cy = realbmp.bmHeight * father->scalingcontrol->scale;
		return 1;
	}
	SIZE sz;
	HFONT font = (HFONT) GetCurrentObject(hdc,OBJ_FONT);
	HFONT prevFont;
	BOOL ggwp = false;
	if (font != father->scalingcontrol->ScaledFont(chr.font))
	{
		prevFont = (HFONT) SelectObject(hdc,father->scalingcontrol->ScaledFont(chr.font));
		ggwp = true;
	}
	GetTextExtentPoint32(hdc,(LPCWSTR)&chr.chr,1, &sz);
	ABC abc;
	GetCharABCWidths(hdc,chr.chr,chr.chr,&abc);
	sz.cx =abc.abcB + abs(abc.abcC);
	if (abc.abcA > 0)
		sz.cx += abc.abcA;
	switch (chr.chr)
	{
	case '\r':
	{
		size->cx = 0;
		size-> cy = 0;
		break;
	}
	case '\t':
	{
		GetTextExtentPoint32(hdc,L"    ",4, &sz);
		size->cx = sz.cx;
		size->cy = sz.cy;
		break;
	}
	default:
	{
		size->cx = sz.cx;
		size->cy = sz.cy;
		break;
	}
	}
	if (ggwp)
		SelectObject(hdc, prevFont);
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
	{
		father->text->DeleteSymbol(firstSelectPosition,secondSelectPosition,TRUE);
		currentPositionToWrite = min(firstSelectPosition,secondSelectPosition);
		firstSelectPosition = currentPositionToWrite;
		secondSelectPosition = currentPositionToWrite;
	}
	return 1;
}
int ActionController::KeyPress(LPARAM lparam, WPARAM wparam)
{
	charRecieved = false;
	switch (wparam)
	{
	case VK_LEFT:
		currentPositionToWrite = max(currentPositionToWrite - 1, 0);
		firstSelectPosition = currentPositionToWrite;
		secondSelectPosition = currentPositionToWrite;
		break;
	case VK_RIGHT:
		currentPositionToWrite = min(currentPositionToWrite +1, father->text->GetSize());
		firstSelectPosition = currentPositionToWrite;
		secondSelectPosition = currentPositionToWrite;
		break;
	case VK_ESCAPE:
		break;
	default:
		charRecieved = true;
		break;
	}
	if (!charRecieved)
		InvalidateRect(father->hWindow->_hwnd, NULL,TRUE);
	return 1;
}
int ActionController::CopyToClipboad(BOOL deleteSelected)
{
	HGLOBAL hglb;
	int size = abs(firstSelectPosition - secondSelectPosition);
	TCHAR* text = new TCHAR[size + 1]; //еще \0
	ExtendedChar *extCharToClipboard = new ExtendedChar[size +1];
	if(OpenClipboard(father->hWindow->_hwnd)&&
		(firstSelectPosition != secondSelectPosition))
    {
		std::vector<ExtendedChar> extchrvctr ;
		father->text->GetData(&extchrvctr);
		EmptyClipboard();
		int j = 0;int k = 0;
		for (int i = min(firstSelectPosition, secondSelectPosition);
			i< max(firstSelectPosition, secondSelectPosition); i++)
		{
			if (extchrvctr[i].bmp == NULL)
			{
				text[j] = extchrvctr[i].chr;
				j++;
			}
			extCharToClipboard[k] = extchrvctr[i];
			k++;
		}
		
		text[j] = '\0';
		ExtendedChar lastChar;
		lastChar.chr = '\0';
		extCharToClipboard[k] = lastChar;
		hglb = GlobalAlloc(GMEM_FIXED, MAX_PATH);
		TCHAR* clip_data = (TCHAR*)GlobalLock(hglb);
		lstrcpy(clip_data,text);
		GlobalUnlock(hglb);
		SetClipboardData(CF_UNICODETEXT, (HANDLE)(clip_data));
		hglb = GlobalAlloc(GMEM_MOVEABLE, sizeof(ExtendedChar) * k + 1);
		ExtendedChar* vasya = (ExtendedChar*) GlobalLock(hglb);
		memcpy(vasya,extCharToClipboard, sizeof(ExtendedChar) * k);
		GlobalUnlock(hglb);
		SetClipboardData(uClipboardFormat, (HANDLE) (extCharToClipboard));
        CloseClipboard();
		if (deleteSelected)
		{
			DeleteSelected();
			InvalidateRect(father->hWindow->_hwnd,NULL,TRUE);
		}
    }
	return 1;
}
int ActionController::PasteFromClipboad()
{
	if(OpenClipboard(father->hWindow->_hwnd))
	{
		std::vector<ExtendedChar> vctr = std::vector<ExtendedChar>();
		ExtendedChar* arraychr;
		if (IsClipboardFormatAvailable(uClipboardFormat))
		{
			 arraychr =(ExtendedChar*) GetClipboardData(uClipboardFormat);
			 for (int i = 0; i < MAXINT; i++)
			 {
				 if (arraychr[i].chr == '\0')
					 break;
				 vctr.push_back(arraychr[i]);
			 }
		}
		else
			if (IsClipboardFormatAvailable(CF_UNICODETEXT))
			{
				TCHAR* text = (TCHAR*) GetClipboardData(CF_UNICODETEXT);
				int i = 0;
				while(1)
				{
					if(text[i] == '\0')
						break;
					ExtendedChar chr= ExtendedChar();
					chr.chr = text[i];
					chr.font = currentFont;
					vctr.push_back(chr);
					i++;
				}
			}
			DeleteSelected();
			father->text->AddVectorExtendedChar(vctr,currentPositionToWrite,TRUE);
			currentPositionToWrite += vctr.size();
			firstSelectPosition = currentPositionToWrite;
			secondSelectPosition =currentPositionToWrite;
			CloseClipboard();
			InvalidateRect(father->hWindow->_hwnd,NULL,TRUE);
	}
	return 1;
}