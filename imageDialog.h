#pragma once
#include "include.h"
#include <wx/filename.h>

class imageDialog : public wxDialog
{
public:
	imageDialog(const wxString& title);

	//elementy edytora
	wxMemoryDC* memoryDC;
	wxClientDC* clientDC;
	wxPanel* mainPanel;
	wxPanel* editorPanel;
	wxButton* saveImageFromEditorBtn;
	wxButton* rotateImageBtn;
	wxButton* loadImageToEditorBtn;
	wxButton* clearEditorBtn;

	//bitmapy do wyswietlania zdjec
	wxBitmap imageBitmap;

	//dane z edytora
	wxFileName fileInEditor;
	wxString fileInEditorExt;

	//metody do obslugi edytora
	void loadImageToEditor(wxCommandEvent& event);
	void saveImage(wxCommandEvent& event);
	void rotate(wxCommandEvent& event);
	void clearEditor(wxCommandEvent& event);
	void scaleImageToEditor(int imageWidth, int imageHeight);
};
