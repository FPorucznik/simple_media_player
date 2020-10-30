#include "mainApp.h"
#include "mFrame.h"

IMPLEMENT_APP(mainApp)

mainApp::mainApp() {
	
}

mainApp::~mainApp() {

}

bool mainApp::OnInit()
{
	mFrame* frame = new mFrame(wxT("SimpleMediaPlayer"));

	frame->Show(true);
	return true;
}
