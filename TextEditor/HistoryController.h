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
	class HistoryElement
	{
	public:
		HistoryElement();
		~HistoryElement();
		std::vector<ExtendedChar> charvector;
		int currentpos;
		int firstpos;
		int secondpos;
		HFONT currentFont;
	};
	int CreateHistoryElement(HistoryElement* el);
	int SetHistoryElement(HistoryElement el);
	int maxStates;
	int currentState;
	std::vector<HistoryElement> states;
	std::vector<HistoryElement> oldStates;
};

