#include "mFrame.h"

mFrame::mFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400)) 
{
	menubar = new wxMenuBar;
	file = new wxMenu;
	file->Append(wxID_ANY, wxT("&Open file"));
	file->Append(wxID_EXIT, wxT("&Quit"));

	menubar->Append(file, wxT("&Player"));
	SetMenuBar(menubar);

	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mFrame::OnQuit));
	Centre();
}

void mFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(true);
}