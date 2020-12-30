#pragma once
#include "include.h"

class imageDialog : public wxDialog
{
public:
	imageDialog(const wxString& title);

	//elementy edytora
	wxMemoryDC* memoryDC;
	wxPanel* mainPanel;
	wxPanel* editorPanel;

	//bitmapy do wyswietlania zdjec
	wxBitmap imageBitmap;
	//wxBitmap tmpImageBitmap;

	//metoda ladujaca zdjecie do edytora
	void loadImageToEditor(wxCommandEvent& event);
};
