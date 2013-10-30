#include "stdafx.h"
#include "AllWhatYouWantController.h"


AllWhatYouWantController::AllWhatYouWantController(TextEditorWindow* window)
{
	hWindow = window;
	actioncontrol = new ActionController(this);
	drawingcontrol = new DrawingController(this);
	fileman = new Filemanager(this);
	text = new Text();
}

AllWhatYouWantController::~AllWhatYouWantController(void)
{
}
