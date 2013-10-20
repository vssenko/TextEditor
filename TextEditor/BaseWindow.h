#pragma once
#include "stdafx.h"
#include <map>
#include <iostream>
#include "TextEditor.h"

class BaseWindow
{
public:
	typedef LRESULT (*FuncPointer)(BaseWindow*, LPARAM,WPARAM); 
	struct POINTER
	{
	BaseWindow* wnd;//Указатель на класс, которому принадлежит обработчик
	FuncPointer func;
	};
	BaseWindow();
	~BaseWindow(void);


	  virtual bool Create(
	HINSTANCE hInstance,
    HWND parent,//Родительское окно, если 0 - то главное окно
    LPCWSTR text,//Заголовок окна
	LPCWSTR className,
    DWORD style, DWORD exStyle,//Стили окна
    int x,int y,int w,int h//Размеры и положение
    ) = 0;

	 public:
		 static LRESULT CALLBACK _WndProc(HWND hwnd,UINT message,WPARAM wparam,	LPARAM lparam);

		 bool AddMessage(UINT message,FuncPointer pointer);
			
  HWND _hwnd;//Хендл нашего окна
  std::map<UINT,POINTER> _msgmap;//Карта сообщений
};

