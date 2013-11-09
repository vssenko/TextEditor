
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
	int SaveBitmap(HANDLE file, HBITMAP bitmap);
	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
	HBITMAP LoadNextImageFromMyFile(HANDLE file);
	void CreateBMPFile(HANDLE file,PBITMAPINFO pbi, 
                  HBITMAP hBMP);
	class FileObject
	{
	public:
		FileObject();
		std::vector<std::pair<HFONT,LOGFONT>> fonts;
		std::vector<std::pair<HBITMAP,BITMAP>> bitmaps;
		std::vector<ExtendedChar> faketext;
	};

};
