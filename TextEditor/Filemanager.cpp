#include "stdafx.h"
#include "Filemanager.h"


Filemanager::Filemanager(void)
{
}


Filemanager::~Filemanager(void)
{
}
HBITMAP Filemanager::LoadMyImage( )
{
	OPENFILENAME openFileStruct = {0};
	TCHAR filename[256] = {0};

	openFileStruct.lStructSize = sizeof(openFileStruct);
	openFileStruct.lpstrFile = filename;
	openFileStruct.nMaxFile = sizeof(filename);

	if (GetOpenFileName(&openFileStruct))
		return (HBITMAP) LoadImage( NULL, openFileStruct.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	else
		return NULL;
}
