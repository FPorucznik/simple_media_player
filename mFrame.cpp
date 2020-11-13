#include "mFrame.h"

//id dla r�nych element�w
enum {
	wxID_MEDIACTRL,
	wxID_BUTTON_VIDEO_LOAD,
	wxID_BUTTON_MUSIC_LOAD,
	wxID_BUTTON_IMAGE_LOAD
};

mFrame::mFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1100, 400)) 
{
	//pasek menu
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
	playerPanel = new wxPanel(panel, wxID_ANY);

	//stworzenie naszego mediaCtrl i umieszczenie go w panelu bocznym
	mediaCtrl = new wxMediaCtrl();
	mediaCtrl->Create(playerPanel, wxID_MEDIACTRL, wxEmptyString, wxDefaultPosition, wxSize(550, 400), 0, wxMEDIABACKEND_WMP10);
	//mediaCtrl->ShowPlayerControls();
	//ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_NONE);
	

	//stworzenie paneli dla ka�dej sekcji
	wxPanel* videoCtrlPanel = new wxPanel(contentTabs);
	wxButton* videoLoadBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_VIDEO_LOAD, wxT("Load video"));

	contentTabs->AddPage(videoCtrlPanel, wxT("Videos"));

	wxPanel* musicCtrlPanel = new wxPanel(contentTabs);
	wxButton* musicLoadBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_MUSIC_LOAD, wxT("Load music"));
	contentTabs->AddPage(musicCtrlPanel, wxT("Music"));

	wxPanel* imagesCtrlPanel = new wxPanel(contentTabs);
	wxButton* imageLoadBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_IMAGE_LOAD, wxT("Load image"));
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
	mediaPlayerSizer->Add(mediaCtrl, 1, wxEXPAND);
	playerPanel->SetSizer(mediaPlayerSizer);
	//------------------------------------------------

	//stworzenie grid sizera do wydzielenia sekcji zak�adek opcji do plki�w oraz sekcji odtwarzacza
	wxGridSizer* panelSizer = new wxGridSizer(1, 2, 0, 0);
	panelSizer->Add(contentTabs, 0, wxEXPAND);
	panelSizer->Add(playerPanel, 1, wxEXPAND | wxALL);
	panel->SetSizer(panelSizer);


	//utworzenie sizera do g��wnego frame'a, na kt�rym znajduje si� ca�y content
	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->SetMinSize(1100, 400);
	topSizer->Add(panel, 1, wxEXPAND);
	SetSizerAndFit(topSizer);


	//po��czenie event�w z przyciskami i odpowiadaj�cymi im metodami (lepszy spos�b ni� tabela event�w)
	Bind(wxEVT_MEDIA_LOADED, &mFrame::OnMediaLoaded, this, wxID_MEDIACTRL);
	Bind(wxEVT_BUTTON, &mFrame::OnVideoOpen, this, wxID_BUTTON_VIDEO_LOAD);
	Bind(wxEVT_BUTTON, &mFrame::OnMusicOpen, this, wxID_BUTTON_MUSIC_LOAD);
	Bind(wxEVT_BUTTON, &mFrame::OnImageOpen, this, wxID_BUTTON_IMAGE_LOAD);
	Bind(wxEVT_MEDIA_LOADED, &mFrame::OnMediaLoaded, this, wxID_MEDIACTRL);
	Bind(wxEVT_MEDIA_FINISHED, &mFrame::OnMediaFinished, this, wxID_MEDIACTRL);

}

//metoda zamykaj�ca program
void mFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(true);
}
//dialogi do otwierania plik�w w zale�no�ci od typu
void mFrame::OnVideoOpen(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog videoFileDialog(this, wxT("Open video file"), "", "", "MP3 and MP4 files (*.mp3;*.mp4)|*.mp3;*.mp4");

	if (videoFileDialog.ShowModal() == wxID_OK) {
		LoadFile(videoFileDialog.GetPath(), "video");
	}
}
void mFrame::OnMusicOpen(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog musicFileDialog(this, wxT("Open music file"), "", "", "MP3,MP4,M4A,WAV files (*.mp3;*.mp4;*.m4a;*.wav)|*.mp3;*.mp4;*.m4a;*.wav");

	if (musicFileDialog.ShowModal() == wxID_OK) {
		LoadFile(musicFileDialog.GetPath(), "music");
	}
}
void mFrame::OnImageOpen(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog imageFileDialog(this, wxT("Open image file"), "", "", "JPEG,PNG,GIF files (*.jpg;*.jpeg;*.png;*.gif)|*.jpg;*.jpeg;*.png;*.gif");

	if (imageFileDialog.ShowModal() == wxID_OK) {
		LoadFile(imageFileDialog.GetPath(), "image");
	}
}

//metoda �aduj�ca plik i wy�wietlaj�ca panel sterowania w zale�no�ci od typu
void mFrame::LoadFile(const wxString& path, const wxString& fileType) {
	if (fileType == "video" || fileType == "music") {
		mediaCtrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_DEFAULT);
		loop = false;
	}
	else {
		mediaCtrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_NONE);
		loop = true;
	}
	mediaCtrl->Load(path);
}

//metoda wywo�ywana gdy zostanie za�adowany plik i b�dzie gotowy do odtworzenia
void mFrame::OnMediaLoaded(wxMediaEvent& WXUNUSED(event)) {
	mediaCtrl->Play();
	position = mediaCtrl->Tell();
}

//metoda wywo�ywana gdy zostanie wykryty koniec odtwarzanie otwartego pliku
void mFrame::OnMediaFinished(wxMediaEvent& WXUNUSED(event)) {
	if (loop) {
		mediaCtrl->Seek(position);
	}
}

mFrame::~mFrame() {
}