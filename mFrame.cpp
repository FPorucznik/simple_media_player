#include "mFrame.h"

mFrame::mFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1100, 400)) 
{

	menubar = new wxMenuBar;
	quit = new wxMenu;
	quit->Append(wxID_EXIT, wxT("&Quit"));

	menubar->Append(quit, wxT("&Player Options"));
	SetMenuBar(menubar);
	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mFrame::OnQuit));

	//utworzenie gl�wnego panelu do trzymania obiekt�w
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	//utworzenie zakladek z r�n� zawarto�ci�
	wxNotebook* contentTabs = new wxNotebook(panel, wxID_ANY);

	//panel na odtwarzacz
	wxPanel* playerPanel = new wxPanel(panel, wxID_ANY);
	wxStaticText* message = new wxStaticText(playerPanel, wxID_ANY, wxT("Media will be displayed here"));
	
	//do cel�w testowych
	testCtrl = new wxTextCtrl(playerPanel, wxID_ANY);


	//testowanie obs�ugi medi�w
	//wxMediaCtrl* media = new wxMediaCtrl(playerPanel, wxID_ANY, wxT("testVid.mp4"));
	//media->ShowPlayerControls();


	//stworzenie paneli dla ka�dej sekcji
	wxPanel* videoCtrlPanel = new wxPanel(contentTabs);
	wxButton* videoLoadBtn = new wxButton(videoCtrlPanel, 1, wxT("Load video"));
	contentTabs->AddPage(videoCtrlPanel, wxT("Videos"));

	wxPanel* musicCtrlPanel = new wxPanel(contentTabs);
	wxButton* musicLoadBtn = new wxButton(musicCtrlPanel, wxID_ANY, wxT("Load music"));
	contentTabs->AddPage(musicCtrlPanel, wxT("Music"));

	wxPanel* imagesCtrlPanel = new wxPanel(contentTabs);
	wxButton* imageLoadBtn = new wxButton(imagesCtrlPanel, wxID_ANY, wxT("Load image"));
	contentTabs->AddPage(imagesCtrlPanel, wxT("Images"));
	//-----------------------------------------------

	//stworzenie sizer�w dla ka�dej zak�adki
	wxBoxSizer* videoTabSizer = new wxBoxSizer(wxVERTICAL);
	videoTabSizer->Add(videoLoadBtn, 0, wxEXPAND);
	videoCtrlPanel ->SetSizer(videoTabSizer);

	wxBoxSizer* musicTabSizer = new wxBoxSizer(wxVERTICAL);
	musicTabSizer->Add(musicLoadBtn, 0, wxEXPAND);
	musicCtrlPanel->SetSizer(musicTabSizer);

	wxBoxSizer* imagesTabSizer = new wxBoxSizer(wxVERTICAL);
	imagesTabSizer->Add(imageLoadBtn, 0, wxEXPAND);
	imagesCtrlPanel->SetSizer(imagesTabSizer);
	//------------------------------------------------

	//sizer dla sekcji odtwarzacza
	wxBoxSizer* mediaPlayerSizer = new wxBoxSizer(wxVERTICAL);
	mediaPlayerSizer->Add(message, 0, wxEXPAND);
	mediaPlayerSizer->Add(testCtrl, 0, wxEXPAND);
	playerPanel->SetSizer(mediaPlayerSizer);
	//------------------------------------------------



	//stworzenie grid sizera do wydzielenia sekcji zak�adek opcji do plki�w oraz sekcji odtwarzacza
	wxGridSizer* panelSizer = new wxGridSizer(1, 2, 0, 0);
	panelSizer->Add(contentTabs, 0, wxEXPAND);
	panelSizer->Add(playerPanel, 1, wxEXPAND);
	panel->SetSizer(panelSizer);


	//utworzenie sizera do g��wnego frame'a, na kt�rym znajduje si� ca�y content
	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->SetMinSize(1100, 400);
	topSizer->Add(panel, 1, wxEXPAND);
	SetSizerAndFit(topSizer);

}

mFrame::~mFrame() {
}

void mFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(true);
}
//testowanie otwierania pliku wideo
void mFrame::OnVideoOpen(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog* openVideoFileDialog = new wxFileDialog(this);

	if (openVideoFileDialog->ShowModal() == wxID_OK) {
		wxString fileName = openVideoFileDialog->GetPath();
		testCtrl->SetValue(fileName);
		delete openVideoFileDialog;
	}
}

//deklaracja tabeli rejestruj�cej wydarzenia i wywo�uj�ca odpowiednie metody
BEGIN_EVENT_TABLE(mFrame, wxFrame)
	EVT_BUTTON(1, mFrame::OnVideoOpen)
END_EVENT_TABLE()