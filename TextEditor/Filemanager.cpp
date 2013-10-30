#include "stdafx.h"
Filemanager::Filemanager(AllWhatYouWantController* controller)
{
	father = controller;
}
Filemanager::~Filemanager(void)
{
}
int Filemanager::LoadMyImage()
{
	OPENFILENAME openFileStruct = {0};
	TCHAR filename[256] = {0};

	openFileStruct.lStructSize = sizeof(openFileStruct);
	openFileStruct.lpstrFile = filename;
	openFileStruct.nMaxFile = sizeof(filename);

	if (GetOpenFileName(&openFileStruct))
	{
		int pos = father->actioncontrol->currentPositionToWrite;
		father->text->AddBitmap((HBITMAP) LoadImage( NULL, openFileStruct.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE), pos);
		return 1;
	}
	else
		return 0;
}
int Filemanager::SaveFile()
{
	return 1;
}
int Filemanager::NewFile()
{
	return 1;
}
int Filemanager::LoadFile()
{
	return 1;
}