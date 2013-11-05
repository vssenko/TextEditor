#include "stdafx.h"
#include "AllWhatYouWantController.h"


AllWhatYouWantController::AllWhatYouWantController(TextEditorWindow* window)
{
	hWindow = window;
	actioncontrol = new ActionController(this);
	drawingcontrol = new DrawingController(this);
	historycontrol = new HistoryController(this);
	scalingcontrol = new ScalingController(this);
	fileman = new Filemanager(this);
	text = new Text(this);
	historycontrol->AddState();
}

AllWhatYouWantController::~AllWhatYouWantController(void)
{
}
