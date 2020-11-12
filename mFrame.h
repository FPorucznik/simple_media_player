#pragma once
#include "include.h"
#include "wx/menu.h"


class mFrame : public wxFrame
{
public:
	mFrame(const wxString& title);
	mFrame();
	~mFrame();

	//metody event�w z menu
	void OnQuit(wxCommandEvent& event);
	
	//metody dotycz�ce player'a
	void OnVideoOpen(wxCommandEvent& event);

	//komponenty menu na pasku
	wxMenuBar* menubar;
	wxMenu* quit;
	
	//kontroler medi�w
	wxMediaCtrl* mediaCtrl;

	//dialog do otwierania plik�w (narazie testowo dodany)
	wxFileDialog* openVideoFileDialog;

	//panel na player'a
	wxPanel* playerPanel;

private:
	//zadeklarowanie tabeli event�w wykorzystywanej przy obs�udze przycisk�w
	DECLARE_EVENT_TABLE();
};

