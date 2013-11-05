

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
	ScalingController* scalingcontrol;
	Text* text;
	TextEditorWindow* hWindow;
};
