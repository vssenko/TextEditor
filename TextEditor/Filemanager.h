#pragma once
static class Filemanager
{
public:
	Filemanager(void);
	~Filemanager(void);
	 static HBITMAP LoadMyImage( );
	 static int SaveFile();
	 static Text* LoadFile();
};

