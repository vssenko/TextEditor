#include "stdafx.h"


BaseWindow::BaseWindow()
{
}

BaseWindow::~BaseWindow(void)
{
}

 LRESULT CALLBACK BaseWindow::_WndProc(HWND hwnd,
		UINT message,
		WPARAM wparam,
		LPARAM lparam)
{
	BaseWindow *wnd=0;
	if(message==WM_NCCREATE)
    {
      //Получаем указатель на экземпляр нашего окна, который мы передали в функцию CreateWindowEx
      wnd=(BaseWindow*)LPCREATESTRUCT(lparam)->lpCreateParams;
      //И сохраняем в поле GWL_USERDATA
      SetWindowLong(hwnd,GWL_USERDATA,LONG(LPCREATESTRUCT(lparam)->lpCreateParams));
      wnd->_hwnd = hwnd;      
    }
    //Теперь получаем указатель на наш экземлпяр окна, но уже из поля GWL_USERDATA
    wnd=(BaseWindow*)GetWindowLong(hwnd,GWL_USERDATA);
    if(wnd)
    {
      //Ищем сообщение в карте
      std::map<UINT,POINTER>::iterator it;
      it=wnd->_msgmap.find(message);

      //Если сообщение не найдено, то обрабатываем его по умолчанию
      if(it==wnd->_msgmap.end()) return DefWindowProc(hwnd,message,wparam,lparam);
      else
      {
        POINTER msg=it->second;        
        //Вызываем функцию обработчик        
        LRESULT result=msg.func(msg.wnd,lparam,wparam);
        if(result) return result;
      }
    }
    return DefWindowProc(hwnd,message,wparam,lparam);
}


 bool BaseWindow::AddMessage(UINT message,FuncPointer pointer)
{
	if(!pointer) return false;

	POINTER msg;
	msg.wnd=this;
	msg.func=pointer;

	_msgmap.insert(std::pair<UINT,POINTER>(message,msg));
return true;
}
