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
	wxID_BUTTON_PLAYLIST_VIDEO_STOP,
	wxID_BUTTON_PLAYLIST_MUSIC_STOP,
	wxID_BUTTON_PLAYLIST_IMAGE_STOP,
	wxID_BUTTON_PLAYLIST_VIDEO_DELETE,
	wxID_BUTTON_PLAYLIST_MUSIC_DELETE,
	wxID_BUTTON_PLAYLIST_IMAGE_DELETE,
	wxID_BUTTON_PLAYLIST_MOVE_UP_VIDEO,
	wxID_BUTTON_PLAYLIST_MOVE_UP_MUSIC,
	wxID_BUTTON_PLAYLIST_MOVE_UP_IMAGE,
	wxID_BUTTON_PLAYLIST_MOVE_DOWN_VIDEO,
	wxID_BUTTON_PLAYLIST_MOVE_DOWN_MUSIC,
	wxID_BUTTON_PLAYLIST_MOVE_DOWN_IMAGE,
	wxID_BUTTON_OPEN_IMAGE_EDITOR
};

mFrame::mFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1100, 400))
{
	//inicjalizacja zmiennych
	loop = false;
	isPlayingFromPlaylist = false;
	position = 0;

	playlistFilesAmount = 0;
	selection = 0;
	playlistFiles.Insert("", 0);
	playlistType = "";

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
	deleteVideoFromPlaylistBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_PLAYLIST_VIDEO_DELETE, wxT("Delete selected"));
	moveUpVideoInPlaylistBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_UP_VIDEO, wxT("Move Up"));
	moveDownVideoInPlaylistBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_DOWN_VIDEO, wxT("Move Down"));
	
	videoPlaylist = new wxListView(videoCtrlPanel, wxID_ANY, wxDefaultPosition, wxSize(500,300));
	videoPlaylist->SetSingleStyle(wxLC_SINGLE_SEL);
	videoPlaylist->AppendColumn("Pos.", wxLIST_FORMAT_LEFT, 50);
	videoPlaylist->AppendColumn("Name", wxLIST_FORMAT_LEFT, 250);
	videoPlaylist->AppendColumn("Path", wxLIST_FORMAT_LEFT, 200);
	contentTabs->AddPage(videoCtrlPanel, wxT("Videos"));


	wxPanel* musicCtrlPanel = new wxPanel(contentTabs);
	wxButton* musicLoadBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_MUSIC_LOAD, wxT("Load music"));
	wxButton* addMusicFileToPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_ADD_MUSIC, wxT("Add music file to playlist"));
	wxButton* playMusicPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_MUSIC_PLAY, wxT("Play music playlist"));
	deleteMusicFromPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_MUSIC_DELETE, wxT("Delete selected"));
	moveUpMusicInPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_UP_MUSIC, wxT("Move Up"));
	moveDownMusicInPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_DOWN_MUSIC, wxT("Move Down"));

	musicPlaylist = new wxListView(musicCtrlPanel, wxID_ANY, wxDefaultPosition, wxSize(500, 300));
	musicPlaylist->SetSingleStyle(wxLC_SINGLE_SEL);
	musicPlaylist->AppendColumn("Pos.", wxLIST_FORMAT_LEFT, 50);
	musicPlaylist->AppendColumn("Name", wxLIST_FORMAT_LEFT, 250);
	musicPlaylist->AppendColumn("Path", wxLIST_FORMAT_LEFT, 200);
	contentTabs->AddPage(musicCtrlPanel, wxT("Music"));

	wxPanel* imagesCtrlPanel = new wxPanel(contentTabs);
	wxButton* imageLoadBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_IMAGE_LOAD, wxT("Load image"));
	wxButton* addImageFileToPlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_ADD_IMAGE, wxT("Add image file to playlist"));
	wxButton* playImagePlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_IMAGE_PLAY, wxT("Slideshow"));
	deleteImageFromPlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_IMAGE_DELETE, wxT("Delete selected"));
	moveUpImageInPlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_UP_IMAGE, wxT("Move Up"));
	moveDownImageInPlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_MOVE_DOWN_IMAGE, wxT("Move Down"));

	openImageEditorBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_OPEN_IMAGE_EDITOR, wxT("Open editor"));

	imagePlaylist = new wxListView(imagesCtrlPanel, wxID_ANY, wxDefaultPosition, wxSize(500, 300));
	imagePlaylist->SetSingleStyle(wxLC_SINGLE_SEL);
	imagePlaylist->AppendColumn("Pos.", wxLIST_FORMAT_LEFT, 50);
	imagePlaylist->AppendColumn("Name", wxLIST_FORMAT_LEFT, 250);
	imagePlaylist->AppendColumn("Path", wxLIST_FORMAT_LEFT, 200);
	contentTabs->AddPage(imagesCtrlPanel, wxT("Images"));
	//-----------------------------------------------

	stopVideoPlaylistBtn = new wxButton(videoCtrlPanel, wxID_BUTTON_PLAYLIST_VIDEO_STOP, wxT("Stop playlist"));
	stopMusicPlaylistBtn = new wxButton(musicCtrlPanel, wxID_BUTTON_PLAYLIST_MUSIC_STOP, wxT("Stop playlist"));
	stopImagePlaylistBtn = new wxButton(imagesCtrlPanel, wxID_BUTTON_PLAYLIST_IMAGE_STOP, wxT("Stop slideshow"));



	//stworzenie sizerow dla kazdej zakladki (videos, music, images)
	wxBoxSizer* videoTabSizer = new wxBoxSizer(wxVERTICAL);
	videoTabSizer->Add(videoLoadBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(addVideoFileToPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(videoPlaylist, 1, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(playVideoPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(stopVideoPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(deleteVideoFromPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(moveUpVideoInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoTabSizer->Add(moveDownVideoInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	videoCtrlPanel->SetSizer(videoTabSizer);

	wxBoxSizer* musicTabSizer = new wxBoxSizer(wxVERTICAL);
	musicTabSizer->Add(musicLoadBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(addMusicFileToPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(musicPlaylist, 1, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(playMusicPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(stopMusicPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(deleteMusicFromPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(moveUpMusicInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicTabSizer->Add(moveDownMusicInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	musicCtrlPanel->SetSizer(musicTabSizer);

	wxBoxSizer* imagesTabSizer = new wxBoxSizer(wxVERTICAL);
	imagesTabSizer->Add(imageLoadBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(addImageFileToPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(imagePlaylist, 1, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(playImagePlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(stopImagePlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(deleteImageFromPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(moveUpImageInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);
	imagesTabSizer->Add(moveDownImageInPlaylistBtn, 0, wxALIGN_CENTER_HORIZONTAL);

	imagesTabSizer->Add(openImageEditorBtn, 0, wxALIGN_CENTER_HORIZONTAL);
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

	
	//event handlery
	Bind(wxEVT_MEDIA_LOADED, &mFrame::OnMediaLoaded, this, wxID_MEDIACTRL);
	Bind(wxEVT_BUTTON, &mFrame::OnVideoOpen, this, wxID_BUTTON_VIDEO_LOAD);
	Bind(wxEVT_BUTTON, &mFrame::OnMusicOpen, this, wxID_BUTTON_MUSIC_LOAD);
	Bind(wxEVT_BUTTON, &mFrame::OnImageOpen, this, wxID_BUTTON_IMAGE_LOAD);
	Bind(wxEVT_MEDIA_LOADED, &mFrame::OnMediaLoaded, this, wxID_MEDIACTRL);
	Bind(wxEVT_MEDIA_FINISHED, &mFrame::OnMediaFinished, this, wxID_MEDIACTRL);
	Bind(wxEVT_BUTTON, &mFrame::OnVideoPlaylistAdd, this, wxID_BUTTON_PLAYLIST_ADD_VIDEO);
	Bind(wxEVT_BUTTON, &mFrame::OnMusicPlaylistAdd, this, wxID_BUTTON_PLAYLIST_ADD_MUSIC);
	Bind(wxEVT_BUTTON, &mFrame::OnImagePlaylistAdd, this, wxID_BUTTON_PLAYLIST_ADD_IMAGE);
	Bind(wxEVT_BUTTON, &mFrame::OnVideoPlaylistPlay, this, wxID_BUTTON_PLAYLIST_VIDEO_PLAY);
	Bind(wxEVT_BUTTON, &mFrame::OnMusicPlaylistPlay, this, wxID_BUTTON_PLAYLIST_MUSIC_PLAY);
	Bind(wxEVT_BUTTON, &mFrame::OnImagePlaylistPlay, this, wxID_BUTTON_PLAYLIST_IMAGE_PLAY);
	Bind(wxEVT_BUTTON, &mFrame::OnVideoPlaylistStop, this, wxID_BUTTON_PLAYLIST_VIDEO_STOP);
	Bind(wxEVT_BUTTON, &mFrame::OnMusicPlaylistStop, this, wxID_BUTTON_PLAYLIST_MUSIC_STOP);
	Bind(wxEVT_BUTTON, &mFrame::OnImagePlaylistStop, this, wxID_BUTTON_PLAYLIST_IMAGE_STOP);
	Bind(wxEVT_BUTTON, &mFrame::OnVideoPlaylistDelete, this, wxID_BUTTON_PLAYLIST_VIDEO_DELETE);
	Bind(wxEVT_BUTTON, &mFrame::OnMusicPlaylistDelete, this, wxID_BUTTON_PLAYLIST_MUSIC_DELETE);
	Bind(wxEVT_BUTTON, &mFrame::OnImagePlaylistDelete, this, wxID_BUTTON_PLAYLIST_IMAGE_DELETE);
	Bind(wxEVT_BUTTON, &mFrame::moveUpVideoInPlaylist, this, wxID_BUTTON_PLAYLIST_MOVE_UP_VIDEO);
	Bind(wxEVT_BUTTON, &mFrame::moveDownVideoInPlaylist, this, wxID_BUTTON_PLAYLIST_MOVE_DOWN_VIDEO);
	Bind(wxEVT_BUTTON, &mFrame::moveUpMusicInPlaylist, this, wxID_BUTTON_PLAYLIST_MOVE_UP_MUSIC);
	Bind(wxEVT_BUTTON, &mFrame::moveDownMusicInPlaylist, this, wxID_BUTTON_PLAYLIST_MOVE_DOWN_MUSIC);
	Bind(wxEVT_BUTTON, &mFrame::moveUpImageInPlaylist, this, wxID_BUTTON_PLAYLIST_MOVE_UP_IMAGE);
	Bind(wxEVT_BUTTON, &mFrame::moveDownImageInPlaylist, this, wxID_BUTTON_PLAYLIST_MOVE_DOWN_IMAGE);
	Bind(wxEVT_BUTTON, &mFrame::openImageEditor, this, wxID_BUTTON_OPEN_IMAGE_EDITOR);

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

//metody odtwarzajace playlisty
void mFrame::OnVideoPlaylistPlay(wxCommandEvent& WXUNUSED(event)) {
	if (mediaCtrl->GetState() == wxMEDIASTATE_PLAYING || mediaCtrl->GetState() == wxMEDIASTATE_PAUSED) {
		mediaCtrl->Stop();
	}
	int length = videoPlaylist->GetItemCount();
	isPlayingFromPlaylist = true;
	playlistFilesAmount = length;
	selection = 0;

	videoPlaylist->Enable();
	musicPlaylist->Enable();
	imagePlaylist->Enable();

	deleteVideoFromPlaylistBtn->Enable();
	deleteMusicFromPlaylistBtn->Enable();
	deleteImageFromPlaylistBtn->Enable();

	moveUpVideoInPlaylistBtn->Enable();
	moveUpMusicInPlaylistBtn->Enable();
	moveUpImageInPlaylistBtn->Enable();
	moveDownVideoInPlaylistBtn->Enable();
	moveDownMusicInPlaylistBtn->Enable();
	moveDownImageInPlaylistBtn->Enable();

	stopVideoPlaylistBtn->Enable();

	if (length == 0) {
		wxMessageBox(wxT("Playlist is empty!"));
	}
	else {
		playlistFiles.Clear();

		long current = 0;
		wxString filePath;
		for (int i = 0; i < length; i++) {
			videoPlaylist->Focus(i);
			current = videoPlaylist->GetFocusedItem();
			filePath = videoPlaylist->GetItemText(current, 2);
			playlistFiles.Insert(filePath, current);
		}
		playlistType = "video";
		LoadFile(playlistFiles.Item(0), "video");
	}
}
void mFrame::OnMusicPlaylistPlay(wxCommandEvent& WXUNUSED(event)) {
	if (mediaCtrl->GetState() == wxMEDIASTATE_PLAYING || mediaCtrl->GetState() == wxMEDIASTATE_PAUSED) {
		mediaCtrl->Stop();
	}
	int length = musicPlaylist->GetItemCount();
	isPlayingFromPlaylist = true;
	playlistFilesAmount = length;
	selection = 0;

	videoPlaylist->Enable();
	musicPlaylist->Enable();
	imagePlaylist->Enable();

	deleteVideoFromPlaylistBtn->Enable();
	deleteMusicFromPlaylistBtn->Enable();
	deleteImageFromPlaylistBtn->Enable();

	moveUpVideoInPlaylistBtn->Enable();
	moveUpMusicInPlaylistBtn->Enable();
	moveUpImageInPlaylistBtn->Enable();
	moveDownVideoInPlaylistBtn->Enable();
	moveDownMusicInPlaylistBtn->Enable();
	moveDownImageInPlaylistBtn->Enable();

	stopMusicPlaylistBtn->Enable();

	if (length == 0) {
		wxMessageBox(wxT("Playlist is empty!"));
	}
	else {
		playlistFiles.Clear();

		long current = 0;
		wxString filePath;
		for (int i = 0; i < length; i++) {
			musicPlaylist->Focus(i);
			current = musicPlaylist->GetFocusedItem();
			filePath = musicPlaylist->GetItemText(current, 2);
			playlistFiles.Insert(filePath, current);
		}
		playlistType = "music";
		LoadFile(playlistFiles.Item(0), "music");
	}
}
void mFrame::OnImagePlaylistPlay(wxCommandEvent& WXUNUSED(event)) {
	if (mediaCtrl->GetState() == wxMEDIASTATE_PLAYING || mediaCtrl->GetState() == wxMEDIASTATE_PAUSED) {
		mediaCtrl->Stop();
	}
	int length = imagePlaylist->GetItemCount();
	isPlayingFromPlaylist = true;
	playlistFilesAmount = length;
	selection = 0;

	videoPlaylist->Enable();
	musicPlaylist->Enable();
	imagePlaylist->Enable();

	deleteVideoFromPlaylistBtn->Enable();
	deleteMusicFromPlaylistBtn->Enable();
	deleteImageFromPlaylistBtn->Enable();

	moveUpVideoInPlaylistBtn->Enable();
	moveUpMusicInPlaylistBtn->Enable();
	moveUpImageInPlaylistBtn->Enable();
	moveDownVideoInPlaylistBtn->Enable();
	moveDownMusicInPlaylistBtn->Enable();
	moveDownImageInPlaylistBtn->Enable();

	stopImagePlaylistBtn->Enable();

	if (length == 0) {
		wxMessageBox(wxT("Playlist is empty!"));
	}
	else {
		playlistFiles.Clear();

		long current = 0;
		wxString filePath;
		for (int i = 0; i < length; i++) {
			imagePlaylist->Focus(i);
			current = imagePlaylist->GetFocusedItem();
			filePath = imagePlaylist->GetItemText(current, 2);
			playlistFiles.Insert(filePath, current);
		}
		playlistType = "image";
		LoadFile(playlistFiles.Item(0), "image");
	}
}

//metoda ladujaca plik i wyswietlajaca panel sterowania w zaleznosci od typu
void mFrame::LoadFile(const wxString& path, const wxString& fileType) {
	if (fileType == "video") {
		mediaCtrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_DEFAULT);
		loop = false;

		deleteVideoFromPlaylistBtn->Disable();
		moveUpVideoInPlaylistBtn->Disable();
		moveDownVideoInPlaylistBtn->Disable();
		stopMusicPlaylistBtn->Disable();
		stopImagePlaylistBtn->Disable();
	}
	else if (fileType == "music") {
		mediaCtrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_DEFAULT);
		loop = false;

		deleteMusicFromPlaylistBtn->Disable();
		moveUpMusicInPlaylistBtn->Disable();
		moveDownMusicInPlaylistBtn->Disable();
		stopVideoPlaylistBtn->Disable();
		stopImagePlaylistBtn->Disable();
	}
	else {
		mediaCtrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_NONE);
		loop = true;

		deleteImageFromPlaylistBtn->Disable();
		moveUpImageInPlaylistBtn->Disable();
		moveDownImageInPlaylistBtn->Disable();
		stopVideoPlaylistBtn->Disable();
		stopMusicPlaylistBtn->Disable();
	}
	mediaCtrl->Load(path);
}

//metoda wywolywana gdy zostanie zaladowany plik i bedzie gotowy do odtworzenia
void mFrame::OnMediaLoaded(wxMediaEvent& WXUNUSED(event)) {
	if (isPlayingFromPlaylist) {
		if (playlistType == "video") { 
			videoPlaylist->Select(selection, true); 
			videoPlaylist->Disable(); 
		}
		else if (playlistType == "music") { 
			musicPlaylist->Select(selection, true); 
			musicPlaylist->Disable();
		}
		else {
			imagePlaylist->Select(selection, true);
			imagePlaylist->Disable();
		}
	}
	mediaCtrl->Play();
	position = mediaCtrl->Tell();
}

//metoda wywolywana gdy zostanie wykryty koniec odtwarzanie otwartego pliku
void mFrame::OnMediaFinished(wxMediaEvent& WXUNUSED(event)) {
	if (isPlayingFromPlaylist) {
		playlistFiles.RemoveAt(0, 1);
		playlistFilesAmount--;

		if (playlistType == "video") { videoPlaylist->Select(selection, false); }
		else if (playlistType == "music") { musicPlaylist->Select(selection, false); }
		else { imagePlaylist->Select(selection, false); }

		if (playlistFilesAmount > 0) {
			selection++;
			mediaCtrl->Load(playlistFiles.Item(0));
		}
		else { 
			isPlayingFromPlaylist = false;

			if (playlistType == "video") { 
				videoPlaylist->Enable();
				deleteVideoFromPlaylistBtn->Enable();
				moveUpVideoInPlaylistBtn->Enable();
				moveDownVideoInPlaylistBtn->Enable();
				stopMusicPlaylistBtn->Enable();
				stopImagePlaylistBtn->Enable();
			}
			else if (playlistType == "music") { 
				musicPlaylist->Enable();
				deleteMusicFromPlaylistBtn->Enable();
				moveUpMusicInPlaylistBtn->Enable();
				moveDownMusicInPlaylistBtn->Enable();
				stopVideoPlaylistBtn->Enable();
				stopImagePlaylistBtn->Enable();
			}
			else {
				imagePlaylist->Enable();
				deleteImageFromPlaylistBtn->Enable();
				moveUpImageInPlaylistBtn->Enable();
				moveDownImageInPlaylistBtn->Enable();
				stopVideoPlaylistBtn->Enable();
				stopMusicPlaylistBtn->Enable();
			}

			mediaCtrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_NONE);
		}
	}
	else {
		if (loop) {
			mediaCtrl->Seek(position);
		}
	}
}
//metody zatrzymujace odtwarzana playliste
void mFrame::OnVideoPlaylistStop(wxCommandEvent& WXUNUSED(event)) {
	mediaCtrl->Stop();
	mediaCtrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_NONE);
	videoPlaylist->Enable();
	deleteVideoFromPlaylistBtn->Enable();
	moveUpVideoInPlaylistBtn->Enable();
	moveDownVideoInPlaylistBtn->Enable();
	stopMusicPlaylistBtn->Enable();
	stopImagePlaylistBtn->Enable();
}
void mFrame::OnMusicPlaylistStop(wxCommandEvent& WXUNUSED(event)) {
	mediaCtrl->Stop();
	mediaCtrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_NONE);
	musicPlaylist->Enable();
	deleteMusicFromPlaylistBtn->Enable();
	moveUpMusicInPlaylistBtn->Enable();
	moveDownMusicInPlaylistBtn->Enable();
	stopVideoPlaylistBtn->Enable();
	stopImagePlaylistBtn->Enable();
}
void mFrame::OnImagePlaylistStop(wxCommandEvent& WXUNUSED(event)) {
	mediaCtrl->Stop();
	mediaCtrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_NONE);
	imagePlaylist->Enable();
	deleteImageFromPlaylistBtn->Enable();
	moveUpImageInPlaylistBtn->Enable();
	moveDownImageInPlaylistBtn->Enable();
	stopVideoPlaylistBtn->Enable();
	stopMusicPlaylistBtn->Enable();
}
//metody do usuwania wybranego elementu na playlistach
void mFrame::OnVideoPlaylistDelete(wxCommandEvent& WXUNUSED(event)) {
	long index = videoPlaylist->GetFirstSelected();
	long count = videoPlaylist->GetItemCount();
	if (index != -1) {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			videoPlaylist->DeleteItem(index);
			count = videoPlaylist->GetItemCount();
			long current = 0;

			for (int i = 0; i < count; i++) {
				wxString pos;
				pos << i + 1;
				videoPlaylist->SetItem(i, 0, pos);
			}
		}
	}
	else {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			wxMessageBox(wxT("No file selected"));
		}
	}
}
void mFrame::OnMusicPlaylistDelete(wxCommandEvent& WXUNUSED(event)) {
	long index = musicPlaylist->GetFirstSelected();
	long count = musicPlaylist->GetItemCount();
	if (index != -1) {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			musicPlaylist->DeleteItem(index);
			count = musicPlaylist->GetItemCount();
			long current = 0;

			for (int i = 0; i < count; i++) {
				wxString pos;
				pos << i + 1;
				musicPlaylist->SetItem(i, 0, pos);
			}
		}
	}
	else {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			wxMessageBox(wxT("No file selected"));
		}
	}

}
void mFrame::OnImagePlaylistDelete(wxCommandEvent& WXUNUSED(event)) {
	long index = imagePlaylist->GetFirstSelected();
	long count = imagePlaylist->GetItemCount();
	if (index != -1) {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			imagePlaylist->DeleteItem(index);
			count = imagePlaylist->GetItemCount();
			long current = 0;

			for (int i = 0; i < count; i++) {
				wxString pos;
				pos << i + 1;
				imagePlaylist->SetItem(i, 0, pos);
			}
		}
	}
	else {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			wxMessageBox(wxT("No file selected"));
		}
	}
}
//metody do przesuwania wybranego elementu na playlistach
void mFrame::moveUpVideoInPlaylist(wxCommandEvent& WXUNUSED(event)) {
	long index = videoPlaylist->GetFirstSelected();
	long count = videoPlaylist->GetItemCount();
	long current_selection = 0;
	if (index != -1) {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			if (count == 1) {
				wxMessageBox(wxT("There is only one file in this playlist!"));
			}
			else {
				wxString selected_name = videoPlaylist->GetItemText(index, 1);
				wxString selected_path = videoPlaylist->GetItemText(index, 2);
				wxString up_name = "";
				wxString up_path = "";

				if (index == 0) {
					wxMessageBox(wxT("The file is already at the top!"));
				}
				else {
					up_name = videoPlaylist->GetItemText(index - 1, 1);
					up_path = videoPlaylist->GetItemText(index - 1, 2);

					videoPlaylist->SetItem(index - 1, 0, wxString(""));
					videoPlaylist->SetItem(index - 1, 1, selected_name);
					videoPlaylist->SetItem(index - 1, 2, selected_path);

					videoPlaylist->SetItem(index, 0, wxString(""));
					videoPlaylist->SetItem(index, 1, up_name);
					videoPlaylist->SetItem(index, 2, up_path);

					current_selection = index - 1;
				}
			}

			for (int i = 0; i < count; i++) {
				wxString pos;
				pos << i + 1;
				videoPlaylist->SetItem(i, 0, pos);
			}
			videoPlaylist->Select(current_selection, true);
		}
	}
	else {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			wxMessageBox(wxT("No file selected"));
		}
	}
}
void mFrame::moveDownVideoInPlaylist(wxCommandEvent& WXUNUSED(event)) {
	long index = videoPlaylist->GetFirstSelected();
	long count = videoPlaylist->GetItemCount();
	long current_selection = 0;
	if (index != -1) {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			if (count == 1) {
				wxMessageBox(wxT("There is only one file in this playlist!"));
			}
			else {
				wxString selected_name = videoPlaylist->GetItemText(index, 1);
				wxString selected_path = videoPlaylist->GetItemText(index, 2);
				wxString down_name = "";
				wxString down_path = "";

				if (index == count - 1) {
					wxMessageBox(wxT("The file is already at the bottom!"));
					current_selection = count - 1;
				}
				else {
					down_name = videoPlaylist->GetItemText(index + 1, 1);
					down_path = videoPlaylist->GetItemText(index + 1, 2);

					videoPlaylist->SetItem(index + 1, 0, wxString(""));
					videoPlaylist->SetItem(index + 1, 1, selected_name);
					videoPlaylist->SetItem(index + 1, 2, selected_path);

					videoPlaylist->SetItem(index, 0, wxString(""));
					videoPlaylist->SetItem(index, 1, down_name);
					videoPlaylist->SetItem(index, 2, down_path);

					current_selection = index + 1;
				}
			}

			for (int i = 0; i < count; i++) {
				wxString pos;
				pos << i + 1;
				videoPlaylist->SetItem(i, 0, pos);
			}
			videoPlaylist->Select(current_selection, true);
		}
	}
	else {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			wxMessageBox(wxT("No file selected"));
		}
	}
}
void mFrame::moveUpMusicInPlaylist(wxCommandEvent& WXUNUSED(event)) {
	long index = musicPlaylist->GetFirstSelected();
	long count = musicPlaylist->GetItemCount();
	long current_selection = 0;
	if (index != -1) {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			if (count == 1) {
				wxMessageBox(wxT("There is only one file in this playlist!"));
			}
			else {
				wxString selected_name = musicPlaylist->GetItemText(index, 1);
				wxString selected_path = musicPlaylist->GetItemText(index, 2);
				wxString up_name = "";
				wxString up_path = "";

				if (index == 0) {
					wxMessageBox(wxT("The file is already at the top!"));
				}
				else {
					up_name = musicPlaylist->GetItemText(index - 1, 1);
					up_path = musicPlaylist->GetItemText(index - 1, 2);

					musicPlaylist->SetItem(index - 1, 0, wxString(""));
					musicPlaylist->SetItem(index - 1, 1, selected_name);
					musicPlaylist->SetItem(index - 1, 2, selected_path);

					musicPlaylist->SetItem(index, 0, wxString(""));
					musicPlaylist->SetItem(index, 1, up_name);
					musicPlaylist->SetItem(index, 2, up_path);

					current_selection = index - 1;
				}
			}

			for (int i = 0; i < count; i++) {
				wxString pos;
				pos << i + 1;
				musicPlaylist->SetItem(i, 0, pos);
			}
			musicPlaylist->Select(current_selection, true);
		}
	}
	else {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			wxMessageBox(wxT("No file selected"));
		}
	}
}
void mFrame::moveDownMusicInPlaylist(wxCommandEvent& WXUNUSED(event)) {
	long index = musicPlaylist->GetFirstSelected();
	long count = musicPlaylist->GetItemCount();
	long current_selection = 0;
	if (index != -1) {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			if (count == 1) {
				wxMessageBox(wxT("There is only one file in this playlist!"));
			}
			else {
				wxString selected_name = musicPlaylist->GetItemText(index, 1);
				wxString selected_path = musicPlaylist->GetItemText(index, 2);
				wxString down_name = "";
				wxString down_path = "";

				if (index == count - 1) {
					wxMessageBox(wxT("The file is already at the bottom!"));
					current_selection = count - 1;
				}
				else {
					down_name = musicPlaylist->GetItemText(index + 1, 1);
					down_path = musicPlaylist->GetItemText(index + 1, 2);

					musicPlaylist->SetItem(index + 1, 0, wxString(""));
					musicPlaylist->SetItem(index + 1, 1, selected_name);
					musicPlaylist->SetItem(index + 1, 2, selected_path);

					musicPlaylist->SetItem(index, 0, wxString(""));
					musicPlaylist->SetItem(index, 1, down_name);
					musicPlaylist->SetItem(index, 2, down_path);

					current_selection = index + 1;
				}
			}

			for (int i = 0; i < count; i++) {
				wxString pos;
				pos << i + 1;
				musicPlaylist->SetItem(i, 0, pos);
			}
			musicPlaylist->Select(current_selection, true);
		}
	}
	else {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			wxMessageBox(wxT("No file selected"));
		}
	}
}
void mFrame::moveUpImageInPlaylist(wxCommandEvent& WXUNUSED(event)) {
	long index = imagePlaylist->GetFirstSelected();
	long count = imagePlaylist->GetItemCount();
	long current_selection = 0;
	if (index != -1) {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			if (count == 1) {
				wxMessageBox(wxT("There is only one file in this playlist!"));
			}
			else {
				wxString selected_name = imagePlaylist->GetItemText(index, 1);
				wxString selected_path = imagePlaylist->GetItemText(index, 2);
				wxString up_name = "";
				wxString up_path = "";

				if (index == 0) {
					wxMessageBox(wxT("The file is already at the top!"));
				}
				else {
					up_name = imagePlaylist->GetItemText(index - 1, 1);
					up_path = imagePlaylist->GetItemText(index - 1, 2);

					imagePlaylist->SetItem(index - 1, 0, wxString(""));
					imagePlaylist->SetItem(index - 1, 1, selected_name);
					imagePlaylist->SetItem(index - 1, 2, selected_path);

					imagePlaylist->SetItem(index, 0, wxString(""));
					imagePlaylist->SetItem(index, 1, up_name);
					imagePlaylist->SetItem(index, 2, up_path);

					current_selection = index - 1;
				}
			}

			for (int i = 0; i < count; i++) {
				wxString pos;
				pos << i + 1;
				imagePlaylist->SetItem(i, 0, pos);
			}
			imagePlaylist->Select(current_selection, true);
		}
	}
	else {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			wxMessageBox(wxT("No file selected"));
		}
	}
}
void mFrame::moveDownImageInPlaylist(wxCommandEvent& WXUNUSED(event)) {
	long index = imagePlaylist->GetFirstSelected();
	long count = imagePlaylist->GetItemCount();
	long current_selection = 0;
	if (index != -1) {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			if (count == 1) {
				wxMessageBox(wxT("There is only one file in this playlist!"));
			}
			else {
				wxString selected_name = imagePlaylist->GetItemText(index, 1);
				wxString selected_path = imagePlaylist->GetItemText(index, 2);
				wxString down_name = "";
				wxString down_path = "";

				if (index == count - 1) {
					wxMessageBox(wxT("The file is already at the bottom!"));
					current_selection = count - 1;
				}
				else {
					down_name = imagePlaylist->GetItemText(index + 1, 1);
					down_path = imagePlaylist->GetItemText(index + 1, 2);

					imagePlaylist->SetItem(index + 1, 0, wxString(""));
					imagePlaylist->SetItem(index + 1, 1, selected_name);
					imagePlaylist->SetItem(index + 1, 2, selected_path);

					imagePlaylist->SetItem(index, 0, wxString(""));
					imagePlaylist->SetItem(index, 1, down_name);
					imagePlaylist->SetItem(index, 2, down_path);

					current_selection = index + 1;
				}
			}

			for (int i = 0; i < count; i++) {
				wxString pos;
				pos << i + 1;
				imagePlaylist->SetItem(i, 0, pos);
			}
			imagePlaylist->Select(current_selection, true);
		}
	}
	else {
		if (count == 0) {
			wxMessageBox(wxT("Playlist is empty!"));
		}
		else {
			wxMessageBox(wxT("No file selected"));
		}
	}
}
//metoda otwierajaca edytor zdjec
void mFrame::openImageEditor(wxCommandEvent& WXUNUSED(event)) {
	imageDialog* imageEditor = new imageDialog(wxT("Image editor"));
	imageEditor->Show(true);
}
mFrame::~mFrame() {
}