#pragma once
#include "include.h"

//g³ówna klasa aplikacji dziedzicz¹ca z klasy oferowanej przez bibliotekê zajmuj¹ca siê uruchamianiem programu
class mainApp : public wxApp
{
public:
	mainApp();
	~mainApp();
	virtual bool OnInit();
};


