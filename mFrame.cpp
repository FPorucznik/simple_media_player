#include "mFrame.h"

mFrame::mFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 600)) 
{
	//------future feature----
	//wxGridSizer* grid = new wxGridSizer(1, 2, 0, 0);
	//this->SetSizer(grid);
	//grid->Layout();

	menubar = new wxMenuBar;
	quit = new wxMenu;
	quit->Append(wxID_EXIT, wxT("&Quit"));

	menubar->Append(quit, wxT("&Player Options"));
	SetMenuBar(menubar);

	tabs = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));
	tabs->AddPage(new wxNotebookPage(tabs, -1), wxT("Videos"));
	tabs->AddPage(new wxNotebookPage(tabs, -1), wxT("Music"));
	tabs->AddPage(new wxNotebookPage(tabs, -1), wxT("Photos"));

	//------future feature----
	//ctr1 = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(0,0));
	//ctr2 = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(0,0));
	//leftPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(300, 300));
	//rightPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(300, 300));
	//rightPanel->SetBackgroundColour(wxColor(*wxBLACK));
	//leftPanel->AddChild(ctr1);

	//------future feature----
	//grid->Add(ctr1, wxEXPAND);
	//grid->Add(ctr2, wxEXPAND);
	//grid->Add(leftPanel);
	//grid->Add(rightPanel);


	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mFrame::OnQuit));
	Centre();
}

mFrame::~mFrame() {
}

void mFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(true);
}