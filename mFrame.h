#pragma once
#include "include.h"
#include "wx/menu.h"


class mFrame : public wxFrame
{
public:
	mFrame(const wxString& title);
	mFrame();
	~mFrame();

	//metody eventow z menu
	void OnQuit(wxCommandEvent& event);
	
	//metody dotyczace player'a
	void OnVideoOpen(wxCommandEvent& event);
	void OnMusicOpen(wxCommandEvent& event);
	void OnImageOpen(wxCommandEvent& event);
	void LoadFile(const wxString& path, const wxString& fileType);
	void OnMediaLoaded(wxMediaEvent& event);
	void OnMediaFinished(wxMediaEvent& event);
	void LoadFileFromPlaylist(const wxString& path, const wxString& playlistType);

	//metody dotyczace playlist
	void OnVideoPlaylistAdd(wxCommandEvent& event);
	void OnMusicPlaylistAdd(wxCommandEvent& event);
	void OnImagePlaylistAdd(wxCommandEvent& event);
	void OnVideoPlaylistPlay(wxCommandEvent& event);
	void OnMusicPlaylistPlay(wxCommandEvent& event);
	void OnImagePlaylistPlay(wxCommandEvent& event);
	void OnVideoPlaylistStop(wxCommandEvent& event);
	void OnMusicPlaylistStop(wxCommandEvent& event);
	void OnImagePlaylistStop(wxCommandEvent& event);
	void OnVideoPlaylistDelete(wxCommandEvent& event);
	void OnMusicPlaylistDelete(wxCommandEvent& event);
	void OnImagePlaylistDelete(wxCommandEvent& event);

	//komponenty menu na pasku
	wxMenuBar* menubar;
	wxMenu* quit;
	
	//kontroler mediow
	wxMediaCtrl* mediaCtrl;

	//przyciski
	wxButton* deleteVideoFromPlaylistBtn;
	wxButton* moveUpVideoInPlaylistBtn;
	wxButton* moveDownVideoInPlaylistBtn;
	wxButton* deleteMusicFromPlaylistBtn;
	wxButton* moveUpMusicInPlaylistBtn;
	wxButton* moveDownMusicInPlaylistBtn;
	wxButton* deleteImageFromPlaylistBtn;
	wxButton* moveUpImageInPlaylistBtn;
	wxButton* moveDownImageInPlaylistBtn;
	wxButton* stopVideoPlaylistBtn;
	wxButton* stopMusicPlaylistBtn;
	wxButton* stopImagePlaylistBtn;

	//playlisty
	wxListView* videoPlaylist;
	wxListView* musicPlaylist;
	wxListView* imagePlaylist;

	//panel na player'a
	wxPanel* playerPanel;

	wxArrayString playlistFiles; //tablica do przechowywania sciezek do plikow z playlist
	int playlistFilesAmount; //ilosc plikow w playliscie
	long selection; //do zaznaczenia aktualnie odtwarzanego pliku
	wxString playlistType; //ktora z playlist jest odtwarzana

	bool isPlayingFromPlaylist; //zmienna boolowska czy media odtwarzane sa z playlisty
	bool loop; //zmienna boolowska czy zapetlamy wyswietlany plik (glownie do zdjêc, które player zamykal po chwili)
	wxFileOffset position; //tutaj przechowamy startowa pozycje w czasie odtwarzanego pliku (dodane rowniez z mysla o zdjeciach aby player ich nie wylaczal po paru seknudach)
};

