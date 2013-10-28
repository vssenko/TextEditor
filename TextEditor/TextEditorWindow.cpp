#pragma once
#include "stdafx.h"

#define ID_NEW 0
#define ID_OPEN 1
#define ID_SAVE 2
#define ID_EXIT 3
#define ID_ABOUT 4
#define ID_CHANGE 5
#define ID_IMAGE 6
TextEditorWindow::TextEditorWindow(void)
{
	text = new Text();
	currentPositionToWrite = 0;
	AddMessage(WM_PAINT, &TextEditorWindow::OnPaint);
	AddMessage(WM_CHAR, &TextEditorWindow::OnCharPress);
	AddMessage(WM_DESTROY, &TextEditorWindow::OnDestroy);
	AddMessage(WM_COMMAND, &TextEditorWindow::OnMenuCommand);
	AddMessage(WM_SIZE, &TextEditorWindow::OnSizeMove);
	AddMessage(WM_LBUTTONDOWN, &TextEditorWindow::OnMouseDown);
	AddMessage(WM_LBUTTONUP, &TextEditorWindow::OnMouseUp);
	AddMessage(WM_MOUSEMOVE, &TextEditorWindow::OnMouseMove);
}

TextEditorWindow::~TextEditorWindow(void)
{
}

//Создание
bool  TextEditorWindow::Create(
	HINSTANCE hInstance,
    HWND parent,//Родительское окно, если 0 - то главное окно
    LPCWSTR text,//Заголовок окна
	LPCWSTR className,
    DWORD style, DWORD exStyle,//Стили окна
    int x,int y,int w,int h//Размеры и положение
    )
{
	const int MAX_LOADSTRING = 100;			

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.lpfnWndProc=WNDPROC(_WndProc);//Оконная процедура
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEXTEDITOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_TEXTEDITOR);
	wcex.lpszClassName	= className;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);
	_hwnd = CreateWindowEx(exStyle, className, text, style,
      x,y,w,h, parent, NULL, hInstance, this);
	ShowWindow(_hwnd, SW_SHOW);
	UpdateWindow(_hwnd);
	//---
	HMENU hMainMenu = CreateMenu();
	HMENU hPopMenu = CreatePopupMenu();
	HMENU hChangeMenu = CreatePopupMenu();
	AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hPopMenu, L"Файл"); 
	AppendMenu(hMainMenu, MF_STRING  | MF_POPUP, (UINT)hChangeMenu, L"Изменить");
    AppendMenu(hMainMenu, MF_STRING, ID_ABOUT, L"Помощь");
	AppendMenu(hPopMenu, MF_STRING, ID_NEW, L"Новый");
	AppendMenu(hPopMenu, MF_STRING, ID_OPEN, L"Открыть");
	AppendMenu(hPopMenu, MF_STRING, ID_SAVE, L"Сохранить");
	AppendMenu(hPopMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hPopMenu, MF_STRING, ID_EXIT, L"Выход");
	AppendMenu(hChangeMenu,MF_STRING, ID_CHANGE, L"Шрифт");
	AppendMenu(hChangeMenu, MF_STRING, ID_IMAGE, L"Загрузить картинку");
	SetMenu(_hwnd, hMainMenu);
	InvalidateRect(_hwnd,NULL,TRUE);
	if(!_hwnd) return false;
    return true;
}

//обработчики
LRESULT TextEditorWindow::OnPaint(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(wnd->_hwnd, &ps);
    int xcoord=0, //ииииикс
		ycoord=0; ///иииигрик
	LPRECT wndRect = new RECT();
	SelectObject(hdc, NULL);
	GetClientRect(wnd->_hwnd, wndRect);
    std::vector<ExtendedChar> extchrvector = reinterpret_cast<TextEditorWindow*>(wnd)->text->GetStringText();
	ExtendedChar walker;
	bool isSelected = reinterpret_cast<TextEditorWindow*>(wnd)->mouseDownPosition != reinterpret_cast<TextEditorWindow*>(wnd)->mouseUpPosition;
	HFONT currentFont;
	SIZE elementSize;
	for(int i= 0; i< extchrvector.size();i++)
	{
		if (i == reinterpret_cast<TextEditorWindow*>(wnd)->currentPositionToWrite)
		{
			DestroyCaret();
			CreateCaret(wnd->_hwnd,NULL,1,16);
			SetCaretPos(xcoord,ycoord);
			ShowCaret(wnd->_hwnd);
		}
		if (isSelected && i == reinterpret_cast<TextEditorWindow*>(wnd)->mouseDownPosition)
		{
			SetBkColor(hdc, RGB(0,0,255));
		}
		if (isSelected && i == reinterpret_cast<TextEditorWindow*>(wnd)->mouseUpPosition)
		{
			SetBkColor(hdc, RGB(255,255,255));
		}
		walker = extchrvector[i];
		if (walker.ifImageThenImageIndex != -1)
		{
			reinterpret_cast<TextEditorWindow*>(wnd)->DrawBitmap(hdc, 
				reinterpret_cast<TextEditorWindow*>(wnd)->text->GetBitmapArray()[walker.ifImageThenImageIndex],
				xcoord,ycoord);
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
	EndPaint(wnd->_hwnd, &ps);
	return 0;
}

LRESULT TextEditorWindow::OnCharPress(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	if (reinterpret_cast<TextEditorWindow*>(wnd)->mouseDownPosition !=
		reinterpret_cast<TextEditorWindow*>(wnd)->mouseUpPosition)
	{
		reinterpret_cast<TextEditorWindow*>(wnd)->text->DeleteSymbol(
		reinterpret_cast<TextEditorWindow*>(wnd)->mouseDownPosition,
		reinterpret_cast<TextEditorWindow*>(wnd)->mouseUpPosition);
		reinterpret_cast<TextEditorWindow*>(wnd)->mouseUpPosition = reinterpret_cast<TextEditorWindow*>(wnd)->mouseDownPosition;
		reinterpret_cast<TextEditorWindow*>(wnd)->currentPositionToWrite = reinterpret_cast<TextEditorWindow*>(wnd)->mouseDownPosition;
	}
	reinterpret_cast<TextEditorWindow*>(wnd)->text->AddChar((TCHAR) wparam,
		reinterpret_cast<TextEditorWindow*>(wnd)->currentPositionToWrite);
	reinterpret_cast<TextEditorWindow*>(wnd)->currentPositionToWrite++;
	InvalidateRect(wnd -> _hwnd, NULL, TRUE);
	return 0;
}

LRESULT TextEditorWindow::OnDestroy(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT TextEditorWindow::OnMenuCommand(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	switch (LOWORD(wparam))
	{
		case ID_SAVE:
			reinterpret_cast<TextEditorWindow*>(wnd)->SaveFile();
			break;

		case ID_EXIT:
			DestroyWindow(wnd -> _hwnd);
			break;

		case ID_OPEN:
			MessageBox(wnd -> _hwnd, L"Hz", L"Hz", 0);
			break;

		case ID_NEW:
			MessageBox(wnd -> _hwnd, L"Mamku", L"Viebali", 0);
			break;

		case ID_CHANGE:
			MessageBox(wnd -> _hwnd, L"Sosi", L"Hui", 0);
			break;

		case ID_ABOUT:
			MessageBox(wnd -> _hwnd, L"Vitalik pizdostradalec",L"About", NULL);
			break;
		case ID_IMAGE:
				Filemanager fileman;
				HBITMAP vasya = fileman.LoadMyImage();
				reinterpret_cast<TextEditorWindow*>(wnd)->text->AddBitmap(vasya, reinterpret_cast<TextEditorWindow*>(wnd)->currentPositionToWrite);
				InvalidateRect(wnd->_hwnd, NULL, TRUE);
			break;
	}
	return 0;
}

LRESULT TextEditorWindow::OnSizeMove(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	return 0;
}

LRESULT TextEditorWindow::OnMouseDown(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	reinterpret_cast<TextEditorWindow*>(wnd)->mouseDownPosition = reinterpret_cast<TextEditorWindow*>(wnd)->CalculatePosition(LOWORD(lparam),HIWORD(lparam));
	return 00000;
}

LRESULT TextEditorWindow::OnMouseMove(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	return 456;
}

LRESULT TextEditorWindow::OnMouseUp(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	reinterpret_cast<TextEditorWindow*>(wnd)->mouseUpPosition = reinterpret_cast<TextEditorWindow*>(wnd)->CalculatePosition(LOWORD(lparam),HIWORD(lparam));
	reinterpret_cast<TextEditorWindow*>(wnd)->currentPositionToWrite = reinterpret_cast<TextEditorWindow*>(wnd)->mouseUpPosition;
	if (reinterpret_cast<TextEditorWindow*>(wnd)->mouseDownPosition > reinterpret_cast<TextEditorWindow*>(wnd)->mouseUpPosition)
	{
		int temp = reinterpret_cast<TextEditorWindow*>(wnd)->mouseDownPosition;
		reinterpret_cast<TextEditorWindow*>(wnd)->mouseDownPosition = reinterpret_cast<TextEditorWindow*>(wnd)->mouseUpPosition;
		reinterpret_cast<TextEditorWindow*>(wnd)->mouseUpPosition = temp;
	}
	InvalidateRect(wnd->_hwnd,NULL,TRUE);
	return 00;
}

// кончились
int TextEditorWindow::LoadMyImage()
{
	Filemanager manager;
	HBITMAP bitmp = manager.LoadMyImage();
	text->AddBitmap(bitmp, currentPositionToWrite);
	return 11;
}

int TextEditorWindow::CalculatePosition(int x, int y)
{
	POINT pt = POINT();
	pt.x = x;
	pt.y = y;
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(_hwnd, &ps);
	int xcoord =0 , ycoord = 0;
	LPRECT wndRect = new RECT();
	GetClientRect(_hwnd, wndRect);
    std::vector<ExtendedChar> extchrvector = text->GetStringText();
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

int TextEditorWindow::SaveFile()
{
	return 1488;
}

int TextEditorWindow::DrawBitmap(HDC hdc,HBITMAP hBitmap, int xStart, int yStart) 
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
