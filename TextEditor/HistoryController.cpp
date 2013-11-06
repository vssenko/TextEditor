#include "stdafx.h"
#include "HistoryController.h"

HistoryController::HistoryElement::HistoryElement()
{
}
HistoryController::HistoryElement::~HistoryElement()
{
}
HistoryController::HistoryController(AllWhatYouWantController* contr)
{
	father = contr;
	maxStates = 15;
}
HistoryController::~HistoryController(void)
{
}
int HistoryController::AddState()
{
	HistoryElement newHistoryElement = HistoryElement();
	CreateHistoryElement(&newHistoryElement);
	states.push_back(newHistoryElement);
	oldStates.clear();
	return 1;
}
int HistoryController::Repeal()
{
	if (states.size() > 0)
	{
		HistoryElement tmp;
		CreateHistoryElement(&tmp);
		oldStates.push_back(tmp);
		tmp = states.back();
		states.pop_back();
		SetHistoryElement(tmp);
		InvalidateRect(father->hWindow->_hwnd,NULL,TRUE);
		return 1;
	}
	return 0;
}
int HistoryController::RepealOfRepeal()
{
	if (oldStates.size() > 0 )
	{
		HistoryElement tmp;
		CreateHistoryElement(&tmp);
		states.push_back(tmp);
		tmp = oldStates.back();
		oldStates.pop_back();
		SetHistoryElement(tmp);
		InvalidateRect(father->hWindow->_hwnd,NULL,TRUE);
		return 1;
	}
	return 0;
}
int HistoryController::ClearHistory()
{
	states = std::vector<HistoryElement>();
	oldStates = std::vector<HistoryElement>();
	return 1;
}
int HistoryController::CreateHistoryElement(HistoryElement* el)
{
	HistoryElement newHistoryElement = HistoryElement();
	father->text->GetData(&newHistoryElement.charvector);
	newHistoryElement.currentFont = father->actioncontrol->currentFont;
	newHistoryElement.currentpos = father->actioncontrol->currentPositionToWrite;
	newHistoryElement.firstpos = father->actioncontrol->firstSelectPosition;
	newHistoryElement.secondpos = father->actioncontrol->secondSelectPosition;
	*el = newHistoryElement;
	return 1;
}
int HistoryController::SetHistoryElement(HistoryElement el)
{
	father->text->SetNewData(el.charvector,FALSE);
	father->actioncontrol->currentFont = el.currentFont;
	father->actioncontrol->currentPositionToWrite = el.currentpos;
	father->actioncontrol->firstSelectPosition = el.firstpos;
	father->actioncontrol->secondSelectPosition = el.secondpos;
	return 1;
}