
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
	float scale;
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
	int SetCaret(int x, int y);
private:
	AllWhatYouWantController* father;
};
