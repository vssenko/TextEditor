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
	controller = new AllWhatYouWantController(this);
	AddMessage(WM_PAINT, &TextEditorWindow::OnPaint);
	AddMessage(WM_CHAR, &TextEditorWindow::OnCharPress);
	AddMessage(WM_DESTROY, &TextEditorWindow::OnDestroy);
	AddMessage(WM_COMMAND, &TextEditorWindow::OnMenuCommand);
	AddMessage(WM_SIZE, &TextEditorWindow::OnSizeMove);
	AddMessage(WM_LBUTTONDOWN, &TextEditorWindow::OnMouseDown);
	AddMessage(WM_LBUTTONUP, &TextEditorWindow::OnMouseUp);
	AddMessage(WM_MOUSEMOVE, &TextEditorWindow::OnMouseMove);
	AddMessage(WM_SETFOCUS, &TextEditorWindow::OnSetFocus);
	AddMessage(WM_KILLFOCUS, &TextEditorWindow::OnKillFocus);
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
	reinterpret_cast<TextEditorWindow*>(wnd)->controller->drawingcontrol->PaintAll(NULL,NULL);
	return 1;
}

LRESULT TextEditorWindow::OnCharPress(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->CharPress(lparam, wparam);
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
			reinterpret_cast<TextEditorWindow*>(wnd)->controller->fileman->SaveFile();
			break;

		case ID_EXIT:
			DestroyWindow(wnd -> _hwnd);
			break;

		case ID_OPEN:
			reinterpret_cast<TextEditorWindow*>(wnd)->controller->fileman->LoadFile();
			break;

		case ID_NEW:
			reinterpret_cast<TextEditorWindow*>(wnd)->controller->fileman->NewFile();
			break;

		case ID_CHANGE:
			reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->ChangeFont();
			break;

		case ID_ABOUT:
			MessageBox(wnd -> _hwnd, L"Vitalik pizdostradalec",L"About", NULL);
			break;
		case ID_IMAGE:
			reinterpret_cast<TextEditorWindow*>(wnd)->controller->fileman->LoadMyImage();
			InvalidateRect(wnd->_hwnd, NULL, TRUE);
			break;
	}
	return 0;
}

LRESULT TextEditorWindow::OnSizeMove(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	return 1;
}

LRESULT TextEditorWindow::OnMouseDown(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	int positionpressed = reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->CalculatePosition(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
	reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->Select( positionpressed, -1);
	reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->isStartedSelect = true;
	return 1;
}

LRESULT TextEditorWindow::OnMouseMove(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	if (reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->isStartedSelect)
	{
		int positionpressed = reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->CalculatePosition(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->Select( -1 , positionpressed);
	}
	return 1;
}

LRESULT TextEditorWindow::OnMouseUp(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	int positionpressed = reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->CalculatePosition(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
	reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->Select( -1 , positionpressed);
	reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->isStartedSelect = false;
	return 1;
}

LRESULT TextEditorWindow::OnSetFocus(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{
	reinterpret_cast<TextEditorWindow*>(wnd)->controller->actioncontrol->SetFocus();
	return 1;
}

LRESULT TextEditorWindow::OnKillFocus(BaseWindow* wnd,LPARAM lparam,WPARAM wparam)
{

	return 1;
}