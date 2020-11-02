#pragma once
#include "include.h"
#include "wx/menu.h"


class mFrame : public wxFrame
{
public:
	mFrame(const wxString& title);
	mFrame();
	~mFrame();

	void OnQuit(wxCommandEvent& event);

	void OnVideoOpen(wxCommandEvent& event);

	wxMenuBar* menubar;
	wxMenu* quit;

	//do testu otwierania plik�w
	wxTextCtrl* testCtrl;

private:
	DECLARE_EVENT_TABLE();
};

