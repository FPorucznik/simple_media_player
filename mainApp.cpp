#include "mainApp.h"
#include "mFrame.h"

//implementacja metod g��wnej klasy, jej zadaniem jest uruchamianie programu

IMPLEMENT_APP(mainApp)

mainApp::mainApp() {
	
}

mainApp::~mainApp() {

}

bool mainApp::OnInit()
{
	wxInitAllImageHandlers();

	mFrame* frame = new mFrame(wxT("SimpleMediaPlayer"));

	frame->Show(true);
	return true;
}
