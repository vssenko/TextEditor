
#pragma 
#include "stdafx.h"
class AllWhatYouWantController;
class ActionController : public BaseController
{
public:
	int firstSelectPosition;
	int secondSelectPosition;
	int currentPositionToWrite;
	BOOL isStartedSelect;
	BOOL isMoveSelected;
	BOOL isSelected;
	HFONT currentFont;

	ActionController(AllWhatYouWantController* contr);
	~ActionController(void);
	int CalculatePosition(int x, int y);
	int CharPress(LPARAM lparam, WPARAM wparam);
	int SpecialKeyPressed(WPARAM key);
	int CalculateExtendCharCoordinates(std::vector<std::pair<ExtendedChar,POINT>>* map);
	int MoveSelected(int pos);
	int Select(int pos1, int pos2);
	int ChangeFont();
	int SetFocus();
	int SelectWord(int pos);
private:
	std::vector<TCHAR> delimiters;
	int GetExtendedElementSize(HDC hdc, ExtendedChar chr, SIZE* size);
	int GetWordSize(HDC hdc, Text* text, int currentpos, SIZE* size);
	BOOL IsDelimiter(ExtendedChar chr);
	int DeleteSelected();
};
