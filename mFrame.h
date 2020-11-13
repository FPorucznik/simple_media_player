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
	void OnMusicOpen(wxCommandEvent& event);
	void OnImageOpen(wxCommandEvent& event);
	void LoadFile(const wxString& path, const wxString& fileType);
	void OnMediaLoaded(wxMediaEvent& event);
	void OnMediaFinished(wxMediaEvent& event);

	//komponenty menu na pasku
	wxMenuBar* menubar;
	wxMenu* quit;
	
	//kontroler medi�w
	wxMediaCtrl* mediaCtrl;

	//panel na player'a
	wxPanel* playerPanel;

	
	bool loop; //zmienna boolowska czy zap�tlamy wy�wietlany plik (g��wnie do zdj��, kt�re player zamyka� po chwili)
	wxFileOffset position; //tutaj przechowamy startow� pozycj� w czasie odtwarzanego pliku (dodane r�wnie� z my�l� o zdj�ciach aby player ich nie wy��cza� po paru seknudach)
};

