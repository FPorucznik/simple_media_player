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
	void OnMusicOpen(wxCommandEvent& event);
	void OnImageOpen(wxCommandEvent& event);
	void LoadFile(const wxString& path, const wxString& fileType);
	void OnMediaLoaded(wxMediaEvent& event);
	void OnMediaFinished(wxMediaEvent& event);

	//komponenty menu na pasku
	wxMenuBar* menubar;
	wxMenu* quit;
	
	//kontroler mediów
	wxMediaCtrl* mediaCtrl;

	//panel na player'a
	wxPanel* playerPanel;

	
	bool loop; //zmienna boolowska czy zapêtlamy wyœwietlany plik (g³ównie do zdjêæ, które player zamyka³ po chwili)
	wxFileOffset position; //tutaj przechowamy startow¹ pozycjê w czasie odtwarzanego pliku (dodane równie¿ z myœl¹ o zdjêciach aby player ich nie wy³¹cza³ po paru seknudach)
};

