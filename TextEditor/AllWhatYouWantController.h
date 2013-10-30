

#include "TextEditorWindow.h"
#include "stdafx.h"
#pragma once
class AllWhatYouWantController
{
public:
	AllWhatYouWantController(TextEditorWindow* window);
	~AllWhatYouWantController(void);
	Filemanager* fileman;
	ActionController* actioncontrol;
	DrawingController* drawingcontrol;
	Text *text;
	TextEditorWindow* hWindow;
};
