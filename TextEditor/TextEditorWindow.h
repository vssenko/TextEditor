#pragma once
#include "stdafx.h"
class TextEditorWindow :
	public BaseWindow
{

public:

	TextEditorWindow(void);
	~TextEditorWindow(void);	
	bool Create(
	HINSTANCE hInstance,
    HWND parent,//Родительское окно, если 0 - то главное окно
    LPCWSTR text,//Заголовок окна
	LPCWSTR className,
    DWORD style, DWORD exStyle,//Стили окна
    int x,int y,int w,int h//Размеры и положение
    );
protected:	
	Text *text;
	int currentPositionToWrite;
	int mouseDownPosition;
	int mouseUpPosition;
	//обработчики ниже
	static LRESULT OnPaint(BaseWindow* wnd,LPARAM lparam,WPARAM wparam);
	static LRESULT OnDestroy(BaseWindow* wnd,LPARAM lparam,WPARAM wparam);
	static LRESULT OnCharPress(BaseWindow* wnd,LPARAM lparam,WPARAM wparam);
	static LRESULT OnMenuCommand(BaseWindow* wnd,LPARAM lparam,WPARAM wparam);
	static LRESULT OnSizeMove(BaseWindow* wnd,LPARAM lparam,WPARAM wparam); 
	static LRESULT OnMouseDown(BaseWindow* wnd,LPARAM lparam,WPARAM wparam); 
	static LRESULT OnMouseMove(BaseWindow* wnd,LPARAM lparam,WPARAM wparam); 
	static LRESULT OnMouseUp(BaseWindow* wnd,LPARAM lparam,WPARAM wparam); 
	//обработчики выше
	 int SaveFile();
	 int OpenFile();
private:
	  int CalculatePosition(int x, int y);
};

