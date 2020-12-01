#include "mFrame.h"

//id dla roznych elementow
enum {
	wxID_MEDIACTRL,
	wxID_BUTTON_VIDEO_LOAD,
	wxID_BUTTON_MUSIC_LOAD,
	wxID_BUTTON_IMAGE_LOAD,
	wxID_BUTTON_PLAYLIST_ADD_VIDEO,
	wxID_BUTTON_PLAYLIST_ADD_MUSIC,
	wxID_BUTTON_PLAYLIST_ADD_IMAGE,
	wxID_BUTTON_PLAYLIST_VIDEO_PLAY,
	wxID_BUTTON_PLAYLIST_MUSIC_PLAY,
	wxID_BUTTON_PLAYLIST_IMAGE_PLAY,
	wxID_BUTTON_PLAYLIST_VIDEO_DELETE,
	wxID_BUTTON_PLAYLIST_MUSIC_DELETE,
	wxID_BUTTON_PLAYLIST_IMAGE_DELETE,
	wxID_BUTTON_PLAYLIST_MOVE_UP_VIDEO,
	wxID_BUTTON_PLAYLIST_MOVE_UP_MUSIC,
	wxID_BUTTON_PLAYLIST_MOVE_UP_IMAGE,
	wxID_BUTTON_PLAYLIST_MOVE_DOWN_VIDEO,
	wxID_BUTTON_PLAYLIST_MOVE_DOWN_MUSIC,
	wxID_BUTTON_PLAYLIST_MOVE_DOWN_IMAGE
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

	//utworzenie glownego panelu do trzymania obiektow
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	//utworzenie zakladek z rozna zawartoscia
	wxNotebook* contentTabs = new wxNotebook(panel, wxID_ANY);

	//panel na odtwarzacz
	playerPanel = new wxPanel(panel, wxID_ANY);

	//stworzenie naszego mediaCtrl i umieszczenie go w panelu bocznym
	mediaCtrl = new wxMediaCtrl();
	mediaCtrl->Create(playerPanel, wxID_MEDIACTRL, wxEmptyString, wxDefaultPosition, wxSize(550, 400), 0, wxMEDIABACKEND_WMP10);


	//stworzenie paneli dla kazdej sekcji (videos, music, images), przyciskow do obslugi oraz okno playlisty
	wxPanel* videoCtrlPanel = new wxPanel(contentTabs);
	wxButton* videoLoadBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_VIDEO_LOAD, wxT("Load video"));
	wxButton* addVideoFileToPlaylistBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_PLAYLIST_ADD_VIDEO, wxT("Add video file to playlist"));
	wxButton* playVideoPlaylistBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_PLAYLIST_VIDEO_PLAY, wxT("Play video playlist"));
	wxButton* deleteVideoFromPlaylistBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_PLAYLIST_VIDEO_DELETE, wxT("Delete selected"));
	wxButton* moveUpVideoInPlaylistBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_UP_VIDEO, wxT("Move Up"));
	wxButton* moveDownVideoInPlaylistBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_DOWN_VIDEO, wxT("Move Down"));
	
	videoPlaylist = new wxListView(videoCtrlPanel, wxID_ANY, wxDefaultPosition, wxSize(500,300), wxLC_REPORT);
	videoPlaylist->AppendColumn("Pos.", wxLIST_FORMAT_LEFT, 50);
	videoPlaylist->AppendColumn("Name", wxLIST_FORMAT_LEFT, 250);
	videoPlaylist->AppendColumn("Path", wxLIST_FORMAT_LEFT, 200);
	contentTabs->AddPage(videoCtrlPanel, wxT("Videos"));


	wxPanel* musicCtrlPanel = new wxPanel(contentTabs);
	wxButton* musicLoadBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_MUSIC_LOAD, wxT("Load music"));
	wxButton* addMusicFileToPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_ADD_MUSIC, wxT("Add music file to playlist"));
	wxButton* playMusicPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_MUSIC_PLAY, wxT("Play music playlist"));
	wxButton* deleteMusicFromPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_MUSIC_DELETE, wxT("Delete selected"));
	wxButton* moveUpMusicInPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_UP_MUSIC, wxT("Move Up"));
	wxButton* moveDownMusicInPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_DOWN_MUSIC, wxT("Move Down"));

	musicPlaylist = new wxListView(musicCtrlPanel, wxID_ANY, wxDefaultPosition, wxSize(500, 300), wxLC_REPORT);
	musicPlaylist->AppendColumn("Pos.", wxLIST_FORMAT_LEFT, 50);
	musicPlaylist->AppendColumn("Name", wxLIST_FORMAT_LEFT, 250);
	musicPlaylist->AppendColumn("Path", wxLIST_FORMAT_LEFT, 200);
	contentTabs->AddPage(musicCtrlPanel, wxT("Music"));

	wxPanel* imagesCtrlPanel = new wxPanel(contentTabs);
	wxButton* imageLoadBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_IMAGE_LOAD, wxT("Load image"));
	wxButton* addImageFileToPlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_ADD_IMAGE, wxT("Add image file to playlist"));
	wxButton* playImagePlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_IMAGE_PLAY, wxT("Slideshow"));
	wxButton* deleteImageFromPlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_IMAGE_DELETE, wxT("Delete selected"));
	wxButton* moveUpImageInPlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_UP_IMAGE, wxT("Move Up"));
	wxButton* moveDownImageInPlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_DOWN_IMAGE, wxT("Move Down"));

	imagePlaylist = new wxListView(imagesCtrlPanel, wxID_ANY, wxDefaultPosition, wxSize(500, 300), wxLC_REPORT);
	imagePlaylist->AppendColumn("Pos.", wxLIST_FORMAT_LEFT, 50);
	imagePlaylist->AppendColumn("Name", wxLIST_FORMAT_LEFT, 250);
	imagePlaylist->AppendColumn("Path", wxLIST_FORMAT_LEFT, 200);
	contentTabs->AddPage(imagesCtrlPanel, wxT("Images"));
	//-----------------------------------------------

	//stworzenie sizerow dla kazdej zakladki (videos, music, images)
	wxBoxSizer* videoTabSizer = new wxBoxSizer(wxVERTICAL);
	videoTabSizer->Add(videoLoadBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(addVideoFileToPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(videoPlaylist, 1, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(playVideoPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(deleteVideoFromPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(moveUpVideoInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(moveDownVideoInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoCtrlPanel->SetSizer(videoTabSizer);

	wxBoxSizer* musicTabSizer = new wxBoxSizer(wxVERTICAL);
	musicTabSizer->Add(musicLoadBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(addMusicFileToPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(musicPlaylist, 1, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(playMusicPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(deleteMusicFromPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(moveUpMusicInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(moveDownMusicInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicCtrlPanel->SetSizer(musicTabSizer);

	wxBoxSizer* imagesTabSizer = new wxBoxSizer(wxVERTICAL);
	imagesTabSizer->Add(imageLoadBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(addImageFileToPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(imagePlaylist, 1, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(playImagePlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(deleteImageFromPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(moveUpImageInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(moveDownImageInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesCtrlPanel->SetSizer(imagesTabSizer);
	//------------------------------------------------

	//sizer dla sekcji odtwarzacza
	wxBoxSizer* mediaPlayerSizer = new wxBoxSizer(wxVERTICAL);
	mediaPlayerSizer->Add(mediaCtrl, 1, wxEXPAND);
	playerPanel->SetSizer(mediaPlayerSizer);
	//------------------------------------------------

	//stworzenie grid sizera do wydzielenia sekcji zakladek opcji do plkiow oraz sekcji odtwarzacza
	wxGridSizer* panelSizer = new wxGridSizer(1, 2, 0, 0);
	panelSizer->Add(contentTabs, 0, wxEXPAND);
	panelSizer->Add(playerPanel, 1, wxEXPAND | wxALL);
	panel->SetSizer(panelSizer);


	//utworzenie sizera do glownego frame'a, na ktorym znajduje sie caly content
	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->SetMinSize(1100, 400);
	topSizer->Add(panel, 1, wxEXPAND);
	SetSizerAndFit(topSizer);

	
	//polaczenie eventow z przyciskami i odpowiadajacymi im metodami (lepszy sposob niz tabela eventow)
	Bind(wxEVT_MEDIA_LOADED, &mFrame::OnMediaLoaded, this, wxID_MEDIACTRL);
	Bind(wxEVT_BUTTON, &mFrame::OnVideoOpen, this, wxID_BUTTON_VIDEO_LOAD);
	Bind(wxEVT_BUTTON, &mFrame::OnMusicOpen, this, wxID_BUTTON_MUSIC_LOAD);
	Bind(wxEVT_BUTTON, &mFrame::OnImageOpen, this, wxID_BUTTON_IMAGE_LOAD);
	Bind(wxEVT_MEDIA_LOADED, &mFrame::OnMediaLoaded, this, wxID_MEDIACTRL);
	Bind(wxEVT_MEDIA_FINISHED, &mFrame::OnMediaFinished, this, wxID_MEDIACTRL);
	Bind(wxEVT_BUTTON, &mFrame::OnVideoPlaylistAdd, this, wxID_BUTTON_PLAYLIST_ADD_VIDEO);
	Bind(wxEVT_BUTTON, &mFrame::OnMusicPlaylistAdd, this, wxID_BUTTON_PLAYLIST_ADD_MUSIC);
	Bind(wxEVT_BUTTON, &mFrame::OnImagePlaylistAdd, this, wxID_BUTTON_PLAYLIST_ADD_IMAGE);

}

//metoda zamykajaca program
void mFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(true);
}
//dialogi do otwierania plikow w zaleznoœci od typu
void mFrame::OnVideoOpen(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog videoFileDialog(this, wxT("Open video file"), "", "", "MP4 files and others (*.mp3;*.mp4)|*.mp3;*.mp4");

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
//dialogi do dodawania plikow do playlisty
void mFrame::OnVideoPlaylistAdd(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog videoFileDialog(this, wxT("Add video file"), "", "", "MP4 files and others (*.mp4)|*.mp4");

	if (videoFileDialog.ShowModal() == wxID_OK) {
		int length = videoPlaylist->GetItemCount();
		if (length == 0) {
			wxListItem entry;
			entry.SetId(length);
			entry.SetMask(wxLIST_MASK_DATA);
			videoPlaylist->InsertItem(entry);
			videoPlaylist->SetItem(length, 0, wxString("1"));
			videoPlaylist->SetItem(length, 1, videoFileDialog.GetFilename());
			videoPlaylist->SetItem(length, 2, videoFileDialog.GetPath());
		}
		else {
			wxListItem entry;
			entry.SetId(length);
			entry.SetMask(wxLIST_MASK_DATA);
			videoPlaylist->InsertItem(entry);
			wxString item_index;
			item_index << length + 1;
			videoPlaylist->SetItem(length, 0, item_index);
			videoPlaylist->SetItem(length, 1, videoFileDialog.GetFilename());
			videoPlaylist->SetItem(length, 2, videoFileDialog.GetPath());
		}
	}
}
void mFrame::OnMusicPlaylistAdd(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog musicFileDialog(this, wxT("Open music file"), "", "", "MP3,MP4,M4A,WAV files (*.mp3;*.mp4;*.m4a;*.wav)|*.mp3;*.mp4;*.m4a;*.wav");

	if (musicFileDialog.ShowModal() == wxID_OK) {
		int length = musicPlaylist->GetItemCount();
		if (length == 0) {
			wxListItem entry;
			entry.SetId(length);
			entry.SetMask(wxLIST_MASK_DATA);
			musicPlaylist->InsertItem(entry);
			musicPlaylist->SetItem(length, 0, wxString("1"));
			musicPlaylist->SetItem(length, 1, musicFileDialog.GetFilename());
			musicPlaylist->SetItem(length, 2, musicFileDialog.GetPath());
		}
		else {
			wxListItem entry;
			entry.SetId(length);
			entry.SetMask(wxLIST_MASK_DATA);
			musicPlaylist->InsertItem(entry);
			wxString item_index;
			item_index << length + 1;
			musicPlaylist->SetItem(length, 0, item_index);
			musicPlaylist->SetItem(length, 1, musicFileDialog.GetFilename());
			musicPlaylist->SetItem(length, 2, musicFileDialog.GetPath());
		}
	}
}
void mFrame::OnImagePlaylistAdd(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog imageFileDialog(this, wxT("Open image file"), "", "", "JPEG,PNG,GIF files (*.jpg;*.jpeg;*.png;*.gif)|*.jpg;*.jpeg;*.png;*.gif");

	if (imageFileDialog.ShowModal() == wxID_OK) {
		int length = imagePlaylist->GetItemCount();
		if (length == 0) {
			wxListItem entry;
			entry.SetId(length);
			entry.SetMask(wxLIST_MASK_DATA);
			imagePlaylist->InsertItem(entry);
			imagePlaylist->SetItem(length, 0, wxString("1"));
			imagePlaylist->SetItem(length, 1, imageFileDialog.GetFilename());
			imagePlaylist->SetItem(length, 2, imageFileDialog.GetPath());
		}
		else {
			wxListItem entry;
			entry.SetId(length);
			entry.SetMask(wxLIST_MASK_DATA);
			imagePlaylist->InsertItem(entry);
			wxString item_index;
			item_index << length + 1;
			imagePlaylist->SetItem(length, 0, item_index);
			imagePlaylist->SetItem(length, 1, imageFileDialog.GetFilename());
			imagePlaylist->SetItem(length, 2, imageFileDialog.GetPath());
		}
	}
}


//metoda ladujaca plik i wyswietlajaca panel sterowania w zaleznosci od typu
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

//metoda wywolywana gdy zostanie zaladowany plik i bedzie gotowy do odtworzenia
void mFrame::OnMediaLoaded(wxMediaEvent& WXUNUSED(event)) {
	mediaCtrl->Play();
	position = mediaCtrl->Tell();
}

//metoda wywolywana gdy zostanie wykryty koniec odtwarzanie otwartego pliku
void mFrame::OnMediaFinished(wxMediaEvent& WXUNUSED(event)) {
	if (loop) {
		mediaCtrl->Seek(position);
	}
}

mFrame::~mFrame() {
}