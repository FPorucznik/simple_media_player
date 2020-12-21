#include "imageDialog.h"

imageDialog::imageDialog(const wxString& title)
	: wxDialog(nullptr, -1, title, wxDefaultPosition, wxSize(800, 600))
{
    wxPanel* panel = new wxPanel(this, -1);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    wxButton* loadImageToEditorBtn = new wxButton(this, wxID_ANY, wxT("Load Image"), wxDefaultPosition);

    hbox->Add(loadImageToEditorBtn, 1);

    vbox->Add(panel, 1);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizer(vbox);

    Centre();
    ShowModal();

    Destroy();
}