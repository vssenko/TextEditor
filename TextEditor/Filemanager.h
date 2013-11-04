
#pragma once
#include "stdafx.h"
class AllWhatYouWantController;
 class Filemanager
{
public:
	Filemanager(AllWhatYouWantController* controller);
	~Filemanager(void);
	  int LoadMyImage();
	  int SaveFile();
	  int LoadFile();
	  int NewFile();
private:
	AllWhatYouWantController* father;
	class FileObject
	{
	public:
		FileObject();
		std::vector<std::pair<HFONT,LOGFONT>> fonts;
		std::vector<std::pair<HBITMAP,BITMAP>> bitmaps;
		std::vector<ExtendedChar> faketext;
	};
};
