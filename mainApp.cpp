#include "mainApp.h"

bool mainApp::OnInit()
{
	mFrame* frame = new mFrame(wxT("SimpleMediaPlayer"));

	frame->Show(true);
	return true;
}