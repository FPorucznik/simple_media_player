#pragma once
#include "include.h"
#include <wx/filename.h>


class imageDialog : public wxDialog
{
public:
	imageDialog(const wxString& title);

	//elementy edytora
	wxMemoryDC* memoryDC;
	wxClientDC* imageClientDC;
	wxPanel* mainPanel;
	wxPanel* editorPanel;
	wxButton* saveImageFromEditorBtn;
	wxButton* rotateImageBtn;
	wxButton* loadImageToEditorBtn;
	wxButton* clearEditorBtn;

	//bitmapy do wyswietlania zdjec
	wxBitmap imageBitmap;
	wxBitmap originalBitmap;

	//dane z edytora
	wxFileName fileInEditor;
	wxString fileInEditorExt;
	wxSize originalImageSize;
	wxSize rotatedImageSize;

	//metody do obslugi edytora
	void loadImageToEditor(wxCommandEvent& event);
	void saveImage(wxCommandEvent& event);
	void rotate(wxCommandEvent& event);
	void clearEditor(wxCommandEvent& event);
	void scaleImageToEditor(int imageWidth, int imageHeight);
	void onMouseLeftMove(wxMouseEvent& event);
	void onMouseLeftPressed(wxMouseEvent& event);
	void onMouseLeftUp(wxMouseEvent& event);

	//zmienne przechowujace stan myszki
	bool isPressed;
	unsigned int rotationState;
	wxPoint mousePosition;
	float heightScaleValue;
	float widthScaleValue;
	float rotatedWidthScale;
	float rotatedHeightScale;
};
