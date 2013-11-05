#include "stdafx.h"
#include "HistoryController.h"


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
	states.push_back(*father->text);
	oldStates.clear();
	return 1;
}
int HistoryController::Repeal()
{
	if (states.size() > 1)
	{
		Text* tmp;
		tmp = &states.back();
		states.pop_back();
		oldStates.push_back(*tmp);
		father->text = &states.back();
		return 1;
	}
	return 0;
}
int HistoryController::RepealOfRepeal()
{
	if (oldStates.size() > 0 )
	{
		Text* tmp;
		tmp = &oldStates.back();
		oldStates.pop_back();
		states.push_back(*tmp);
		father->text = &states.back();
		return 1;
	}
	return 0;
}
int HistoryController::ClearHistory()
{
	states = std::vector<Text>();
	oldStates = std::vector<Text>();
	return 1;
}