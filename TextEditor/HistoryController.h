#pragma once
#pragma once

#include "stdafx.h"
class AllWhatYouWantController;
class HistoryController
{
public:
	HistoryController(AllWhatYouWantController* contr);
	~HistoryController(void);
private:
	AllWhatYouWantController* father;
};

