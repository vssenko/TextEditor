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
	OPENFILENAME *filename;
	GetOpenFileName(filename);
	return (HBITMAP) LoadImage( NULL, filename->lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}
