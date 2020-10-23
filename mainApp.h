#pragma once
#include "include.h"
#include "mFrame.h"

class mainApp : public wxApp
{
public:
	virtual bool OnInit();
};

DECLARE_APP(mainApp)

IMPLEMENT_APP(mainApp)

