#pragma once
#include "include.h"
#include "wx/menu.h"
#include <wx/notebook.h>

class mFrame : public wxFrame
{
public:
	mFrame(const wxString& title);
	mFrame();
	~mFrame();

	void OnQuit(wxCommandEvent& event);

	wxMenuBar* menubar;
	wxMenu* quit;
	wxNotebook* tabs;
};

