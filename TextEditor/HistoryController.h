#pragma once
#pragma once

#include "stdafx.h"
class AllWhatYouWantController;
class HistoryController: public BaseController
{
public:
	HistoryController(AllWhatYouWantController* contr);
	~HistoryController(void);
	int AddState();
	int Repeal();
	int RepealOfRepeal(); // ^_^
	int ClearHistory();
private:
	int maxStates;
	int currentState;
	std::vector<Text> states;
	std::vector<Text> oldStates;
};

