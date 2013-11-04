

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
	HistoryController* historycontrol;
	Text* text;
	TextEditorWindow* hWindow;
};
