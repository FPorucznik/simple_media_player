#pragma once
#include "include.h"
#include "wx/menu.h"


class mFrame : public wxFrame
{
public:
	mFrame(const wxString& title);
	mFrame();
	~mFrame();

	//metody eventów z menu
	void OnQuit(wxCommandEvent& event);
	
	//metody dotycz¹ce player'a
	void OnVideoOpen(wxCommandEvent& event);

	//komponenty menu na pasku
	wxMenuBar* menubar;
	wxMenu* quit;
	
	//kontroler mediów
	wxMediaCtrl* mediaCtrl;

	//dialog do otwierania plików (narazie testowo dodany)
	wxFileDialog* openVideoFileDialog;

	//panel na player'a
	wxPanel* playerPanel;

private:
	//zadeklarowanie tabeli eventów wykorzystywanej przy obs³udze przycisków
	DECLARE_EVENT_TABLE();
};

