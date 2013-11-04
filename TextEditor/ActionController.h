
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
	BOOL isMoveSelected;
	BOOL isSelected;
	HFONT currentFont;
	double scale;
	ActionController(AllWhatYouWantController* contr);
	~ActionController(void);
	int CalculatePosition(int x, int y);
	int CharPress(LPARAM lparam, WPARAM wparam);
	int SpecialKeyPressed(WPARAM key);
	int CalculateExtendCharCoordinates(std::vector<std::pair<ExtendedChar,POINT>>* map);
	int MoveSelected(int pos);
	int Select(int pos1, int pos2);
	int IncreaseScale();
	int DecreaseScale();
	int ChangeFont();
	int SetFocus();
	int SelectWord(int pos);
private:
	std::vector<TCHAR> delimiters;
	AllWhatYouWantController* father;
	int GetExtendedElementSize(HDC hdc, ExtendedChar chr, SIZE* size);
	int GetWordSize(HDC hdc, Text* text, int currentpos, SIZE* size);
	BOOL IsDelimiter(ExtendedChar chr);
	int DeleteSelected();
};
