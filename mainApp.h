#pragma once
#include "include.h"

//glowna klasa aplikacji dziedziczaca z klasy oferowanej przez biblioteke zajmuj�ca sie uruchamianiem programu
class mainApp : public wxApp
{
public:
	mainApp();
	~mainApp();
	virtual bool OnInit();
};


