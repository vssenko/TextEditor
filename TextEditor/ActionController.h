
#pragma 
#include "stdafx.h"
class AllWhatYouWantController;
class ActionController
{
public:
	int firstSelectPosition;
	int secondSelectPosition;
	int currentPositionToWrite;
	BOOL isStartedSelect;
	HFONT currentFont;
	double scale;
	ActionController(AllWhatYouWantController* contr);
	~ActionController(void);
	int CalculatePosition(int x, int y);
	int CharPress(LPARAM lparam, WPARAM wparam);
	int SpecialKeyPressed(WPARAM key);
	int MoveSelected();
	int Select(int pos1, int pos2);
	int IncreaseScale();
	int DecreaseScale();
	int ChangeFont();
	int SetFocus();
private:
	AllWhatYouWantController* father;
};
