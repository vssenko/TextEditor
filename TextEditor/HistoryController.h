#pragma once
#pragma once

#include "stdafx.h"
class AllWhatYouWantController;
class HistoryController
{
public:
	HistoryController(AllWhatYouWantController* contr);
	~HistoryController(void);
	int AddState();
	int Repeal();
	int RepealOfRepeal(); // ^_^
	
private:
	int maxStates;
	int currentState;
	std::vector<Text> states;
	std::vector<Text> oldStates;
	AllWhatYouWantController* father;
};

