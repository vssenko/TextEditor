
#pragma once
#include "stdafx.h"
class AllWhatYouWantController;
 class Filemanager: public BaseController
{
public:
	Filemanager(AllWhatYouWantController* controller);
	~Filemanager(void);
	  int LoadMyImage();
	  int SaveFile();
	  int LoadFile();
	  int NewFile();
private:
	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
	class ImageJunk
	{

	};
	class FileObject
	{
	public:
		FileObject();
		std::vector<std::pair<HFONT,LOGFONT>> fonts;
		std::vector<std::pair<HBITMAP,BITMAPINFOHEADER>> bitmaps;
		std::vector<ExtendedChar> faketext;
	};

};
