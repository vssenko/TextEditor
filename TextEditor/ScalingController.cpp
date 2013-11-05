#include "stdafx.h"
ScalingController::ScalingController(AllWhatYouWantController* contr)
{
	father = contr;
	scaledfonts = std::map<HFONT,HFONT>();
	scale = 1;
}
ScalingController::~ScalingController(void)
{
}
int ScalingController::IncreaseScale()
{
	int oldscale = scale;
	scale+= 0.1;
	if (scale > 2)
		scale = 2;
	if (oldscale != scale)
		CalculateNewScaledFonts();
	InvalidateRect(father->hWindow->_hwnd, NULL,TRUE);
	return 1;
}
int ScalingController::DecreaseScale()
{
	int oldscale = scale;
	scale-= 0.1;
	if (scale <0.5)
		scale = 0.5;
	if (oldscale != scale)
		CalculateNewScaledFonts();
	InvalidateRect(father->hWindow->_hwnd, NULL,TRUE);
	return 1;
}
int ScalingController::AddNewFont(HFONT hfont)
{
	LOGFONT scaledfont;
	ZeroMemory(&scaledfont,sizeof(LOGFONT));
	GetObject(hfont,sizeof(LOGFONT), &scaledfont);
	scaledfont.lfHeight = scaledfont.lfHeight * scale;
	scaledfont.lfWidth = scaledfont.lfWidth * scale;
	scaledfonts[hfont] = CreateFontIndirect(&scaledfont);
	return 1;
}
int ScalingController::CalculateNewScaledFonts()
{
	std::map<HFONT,HFONT>::iterator it;
	for(it = scaledfonts.begin(); it != scaledfonts.end(); it++) 
	{
		AddNewFont(it->first);
	}
	return 1;
}
HFONT ScalingController::ScaledFont(HFONT hfont)
{
	if (scaledfonts.find(hfont)== scaledfonts.end())
	{
		AddNewFont(hfont);
	}
	return scaledfonts[hfont];
}