#include "imageDialog.h"

//id dla roznych elementow
enum {
    wxID_BUTTON_EDITOR_LOAD,
    wxID_BUTTON_EDITOR_SAVE,
    wxID_BUTTON_EDITOR_ROTATE,
    wxID_BUTTON_EDITOR_CLEAR
};

imageDialog::imageDialog(const wxString& title)
	: wxDialog(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 600))
{
    //panele do layoutu edytora
    mainPanel = new wxPanel(this, -1);
    editorPanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE);

    clientDC = new wxClientDC(editorPanel);
    memoryDC = new wxMemoryDC();

    //stworzenie sizerow do rozmieszczenia elementow edytora
    wxBoxSizer* vsizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* hsizerEditor = new wxBoxSizer(wxHORIZONTAL);
    hsizerEditor->Add(editorPanel, 1, wxEXPAND);
    vsizer->Add(hsizerEditor, 1, wxLEFT | wxRIGHT | wxEXPAND, 50);
    vsizer->Add(-1, 50);

    wxBoxSizer* hsizerControls = new wxBoxSizer(wxHORIZONTAL);
    loadImageToEditorBtn = new wxButton(mainPanel, wxID_BUTTON_EDITOR_LOAD, wxT("Load image"));
    saveImageFromEditorBtn = new wxButton(mainPanel, wxID_BUTTON_EDITOR_SAVE, wxT("Save image"));
    rotateImageBtn = new wxButton(mainPanel, wxID_BUTTON_EDITOR_ROTATE, wxT("Rotate image"));
    clearEditorBtn = new wxButton(mainPanel, wxID_BUTTON_EDITOR_CLEAR, wxT("Clear editor"));
    hsizerControls->Add(loadImageToEditorBtn);
    hsizerControls->Add(saveImageFromEditorBtn);
    hsizerControls->Add(rotateImageBtn);
    hsizerControls->Add(clearEditorBtn);
    vsizer->Add(hsizerControls, 0);
    vsizer->Add(-1, 50);

    mainPanel->SetSizer(vsizer);

    //wstepne zablokowanie przyciskow
    saveImageFromEditorBtn->Disable();
    rotateImageBtn->Disable();
    clearEditorBtn->Disable();

    //event handlery
    Bind(wxEVT_BUTTON, &imageDialog::loadImageToEditor, this, wxID_BUTTON_EDITOR_LOAD);
    Bind(wxEVT_BUTTON, &imageDialog::saveImage, this, wxID_BUTTON_EDITOR_SAVE);
    Bind(wxEVT_BUTTON, &imageDialog::rotate, this, wxID_BUTTON_EDITOR_ROTATE);
    Bind(wxEVT_BUTTON, &imageDialog::clearEditor, this, wxID_BUTTON_EDITOR_CLEAR);

    Centre();
    ShowModal();
    delete clientDC;
    delete memoryDC;
    Destroy();
}
//metoda ladujaca zdjecie i wczytujaca je do edytora
void imageDialog::loadImageToEditor(wxCommandEvent& WXUNUSED(event)) {
    wxFileDialog imageFileEditorDialog(this, wxT("Open image file"), "", "", "JPEG,PNG,GIF files (*.jpg;*.jpeg;*.png;*.gif)|*.jpg;*.jpeg;*.png;*.gif");

    clientDC->Clear();

    if (imageFileEditorDialog.ShowModal() == wxID_OK) {
        imageBitmap.LoadFile(imageFileEditorDialog.GetPath(), wxBITMAP_TYPE_ANY);
        fileInEditor = imageFileEditorDialog.GetPath();
        fileInEditorExt = fileInEditor.GetExt().Lower();

        scaleImageToEditor(imageBitmap.GetWidth(), imageBitmap.GetHeight());
        clientDC->DrawBitmap(imageBitmap, 0, 0, false);

        saveImageFromEditorBtn->Enable();
        rotateImageBtn->Enable();
        clearEditorBtn->Enable();
    }
}
//metoda zapisujaca edytowane zdjecie na dysku
void imageDialog::saveImage(wxCommandEvent& WXUNUSED(event)) {

    wxSize imageSize(imageBitmap.GetWidth(), imageBitmap.GetHeight());
    wxBitmap tmpImageBitmap(imageSize, wxBITMAP_SCREEN_DEPTH);
    memoryDC->SelectObject(tmpImageBitmap);
    memoryDC->Blit(wxPoint(0, 0), imageSize, clientDC, wxPoint(-1, -1), wxCOPY, true, wxDefaultPosition);

    wxImage convertedImage = imageBitmap.ConvertToImage();
    convertedImage.SetOption(wxIMAGE_OPTION_QUALITY, 100);

    wxFileDialog imageSaveDialog(this, wxT("Save file"), "", "", (fileInEditorExt.Upper() + " files|*." + fileInEditorExt), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition, wxDefaultSize);

    if (imageSaveDialog.ShowModal() == wxID_OK) {
        if (fileInEditorExt == wxT("png")) {
            convertedImage.SaveFile(imageSaveDialog.GetPath(), wxBITMAP_TYPE_PNG);
        }
        else if (fileInEditorExt == wxT("jpg") || fileInEditorExt == wxT("jpeg")) {
            convertedImage.SaveFile(imageSaveDialog.GetPath(), wxBITMAP_TYPE_JPEG);
        }
        else if (fileInEditorExt == wxT("gif")) {
            convertedImage.SaveFile(imageSaveDialog.GetPath(), wxBITMAP_TYPE_GIF);
        }
    }
}
//metoda obracajaca zdjecie
void imageDialog::rotate(wxCommandEvent& WXUNUSED(event)) {
    clientDC->Clear();

    wxImage tmpImage = imageBitmap.ConvertToImage();
    tmpImage = tmpImage.Rotate90(true);
    wxBitmap rotatedBitmap(tmpImage);
    imageBitmap = rotatedBitmap;

    scaleImageToEditor(rotatedBitmap.GetWidth(), rotatedBitmap.GetHeight());
    clientDC->DrawBitmap(rotatedBitmap, 0, 0, false);
}
//metoda sluzaca do przeskalowania zdjecia do rozmiaru edytora
void imageDialog::scaleImageToEditor(int imageWidth, int imageHeight) {
    //zmienne uzyte do przeskalowania obrazow do rozmiaru edytora
    float widthScale = 1.0f;   //skala w poziomie
    float heightScale = 1.0f;  //skala w pionie
    int imageH = imageHeight;  //wysokosc obrazu
    int imageW = imageWidth;   //szerokosc obrazu
    int panelH = 0;            //wysokosc panelu
    int panelW = 0;            //szerokosc panelu

    GetSize(&panelW, &panelH);

    if ((imageH > 0) && (imageW > 0)) {
        //obliczenie wartosci do skalowania obrazu z odjeciem odpowiednich wartosci zwiazanych z borderami
        heightScale = ((float)panelH - 165) / (float)imageH;
        widthScale = ((float)panelW - 119) / (float)imageW;
    }

    clientDC->SetUserScale(widthScale, heightScale);
}
//metoda czyszczaca edytor
void imageDialog::clearEditor(wxCommandEvent& WXUNUSED(event)) {
    clientDC->Clear();
    saveImageFromEditorBtn->Disable();
    rotateImageBtn->Disable();
    clearEditorBtn->Disable();
}
