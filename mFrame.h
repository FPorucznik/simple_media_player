#pragma once
#include "include.h"
#include "wx/menu.h"

class mFrame : public wxFrame
{
public:
	mFrame(const wxString& title);

	void OnQuit(wxCommandEvent& event);

	wxMenuBar* menubar;
	wxMenu* file;
	wxMenu* imp;
};

