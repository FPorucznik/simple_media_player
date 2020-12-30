#include "imageDialog.h"

//id dla roznych elementow
enum {
    wxID_BUTTON_EDITOR_LOAD
};

imageDialog::imageDialog(const wxString& title)
	: wxDialog(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 600))
{
    //panele do layoutu edytora
    mainPanel = new wxPanel(this, -1);
    editorPanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE);

    //stworzenie sizerow do rozmieszczenia elementow edytora
    wxBoxSizer* vsizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* hsizerEditor = new wxBoxSizer(wxHORIZONTAL);
    hsizerEditor->Add(editorPanel, 1, wxEXPAND);
    vsizer->Add(hsizerEditor, 1, wxLEFT | wxRIGHT | wxEXPAND, 50);
    vsizer->Add(-1, 50);

    wxBoxSizer* hsizerControls = new wxBoxSizer(wxHORIZONTAL);
    wxButton* loadImageToEditorBtn = new wxButton(mainPanel, wxID_BUTTON_EDITOR_LOAD, wxT("Load image"));
    hsizerControls->Add(loadImageToEditorBtn);
    vsizer->Add(hsizerControls, 0);
    vsizer->Add(-1, 50);

    mainPanel->SetSizer(vsizer);

    //event handlery
    Bind(wxEVT_BUTTON, &imageDialog::loadImageToEditor, this, wxID_BUTTON_EDITOR_LOAD);

    Centre();
    ShowModal();
    Destroy();
}
//metoda ladujaca zdjecie i wczytujaca je do edytora
void imageDialog::loadImageToEditor(wxCommandEvent& WXUNUSED(event)) {
    wxFileDialog imageFileEditorDialog(this, wxT("Open image file"), "", "", "JPEG,PNG,GIF files (*.jpg;*.jpeg;*.png;*.gif)|*.jpg;*.jpeg;*.png;*.gif");

    wxClientDC dc(editorPanel);

    //zmienne uzyte do przeskalowania obrazow do rozmiaru edytora
    float widthScale = 1.0f;    //skala w poziomie
    float heightScale = 1.0f;   //skala w pionie
    int imageH = -1;            //wysokosc obrazu
    int imageW = -1;            //szerokosc obrazu
    int panelH = -1;            //wysokosc panelu
    int panelW = -1;            //szerokosc panelu

    if (imageFileEditorDialog.ShowModal() == wxID_OK) {
        imageBitmap.LoadFile(imageFileEditorDialog.GetPath(), wxBITMAP_TYPE_ANY);

        //uzyskanie wartosci potrzebnych do przeskalowania obrazu
        imageH = imageBitmap.GetHeight();
        imageW = imageBitmap.GetWidth();

        GetSize(&panelW, &panelH);

        if ((imageH > 0) && (imageW > 0)) {
            //obliczenie wartosci do skalowania obrazu
            heightScale = ((float)panelH - 165) / (float)imageH;
            widthScale = ((float)panelW - 100) / (float)imageW;

            if (heightScale < widthScale) { 
                widthScale = heightScale;
            }
            else { 
                heightScale = widthScale;
            }
        }

        dc.SetUserScale(heightScale, widthScale);
        dc.DrawBitmap(imageBitmap, 0, 0, false);
    }
}
