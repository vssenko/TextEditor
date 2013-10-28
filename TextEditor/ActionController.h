#include "stdafx.h"
#pragma once
class ActionController
{
public:
	int firstSelectPosition;
	int secondSelectPosition;
	int currentPositionToWrite;
	BOOL isStartedSelect;
	HFONT currentFont;
	int scale;
	ActionController(AllWhatYouWantController* contr);
	~ActionController(void);
	int CalculatePosition(int x, int y);
	int CharPress(TCHAR chr);
	int SpecialKeyPressed(WPARAM key);
	int MoveSelected();
	int Select(int pos1, int pos2);
	int IncreaseScale();
	int DecreaseScale();
	int ChangeFont();
private:
	AllWhatYouWantController* father;
};

