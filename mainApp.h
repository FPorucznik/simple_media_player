#pragma once
#include "include.h"

//g��wna klasa aplikacji dziedzicz�ca z klasy oferowanej przez bibliotek� zajmuj�ca si� uruchamianiem programu
class mainApp : public wxApp
{
public:
	mainApp();
	~mainApp();
	virtual bool OnInit();
};


