#pragma once
#include "stdafx.h"
class AllWhatYouWantController;
class ScalingController : public BaseController
{
public:
	double scale;
	ScalingController(AllWhatYouWantController* contr);
	~ScalingController(void);
	int IncreaseScale();
	int DecreaseScale();
	int CalculateNewScaledFonts();
	HFONT ScaledFont(HFONT hfont);
private:
	std::map<HFONT, HFONT> scaledfonts;
	int AddNewFont(HFONT hfont);
};

