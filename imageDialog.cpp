#include "imageDialog.h"

//id dla roznych elementow
enum {
    wxID_EDITOR_PANEL,
    wxID_BUTTON_EDITOR_LOAD,
    wxID_BUTTON_EDITOR_SAVE,
    wxID_BUTTON_EDITOR_ROTATE,
    wxID_BUTTON_EDITOR_CLEAR
};

imageDialog::imageDialog(const wxString& title)
	: wxDialog(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(660, 670))
{
    //panele do layoutu edytora
    mainPanel = new wxPanel(this, -1);
    editorPanel = new wxPanel(mainPanel, wxID_EDITOR_PANEL, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE);

    imageClientDC = new wxClientDC(editorPanel);
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

    isPressed = false;
    rotationState = 0;

    //wstepne zablokowanie przyciskow
    saveImageFromEditorBtn->Disable();
    rotateImageBtn->Disable();
    clearEditorBtn->Disable();

    //event handlery
    Bind(wxEVT_BUTTON, &imageDialog::loadImageToEditor, this, wxID_BUTTON_EDITOR_LOAD);
    Bind(wxEVT_BUTTON, &imageDialog::saveImage, this, wxID_BUTTON_EDITOR_SAVE);
    Bind(wxEVT_BUTTON, &imageDialog::rotate, this, wxID_BUTTON_EDITOR_ROTATE);
    Bind(wxEVT_BUTTON, &imageDialog::clearEditor, this, wxID_BUTTON_EDITOR_CLEAR);
    editorPanel->Bind(wxEVT_LEFT_DOWN, &imageDialog::onMouseLeftPressed, this);
    editorPanel->Bind(wxEVT_LEFT_UP, &imageDialog::onMouseLeftUp, this);
    editorPanel->Bind(wxEVT_MOTION, &imageDialog::onMouseLeftMove, this);

    Centre();
    ShowModal();
    delete imageClientDC;
    delete memoryDC;
    Destroy();
}
//metoda ladujaca zdjecie i wczytujaca je do edytora
void imageDialog::loadImageToEditor(wxCommandEvent& WXUNUSED(event)) {
    wxFileDialog imageFileEditorDialog(this, wxT("Open image file"), "", "", "JPEG,PNG,GIF files (*.jpg;*.jpeg;*.png;*.gif)|*.jpg;*.jpeg;*.png;*.gif");

    imageClientDC->Clear();
    memoryDC->Clear();

    if (imageFileEditorDialog.ShowModal() == wxID_OK) {
        imageBitmap.LoadFile(imageFileEditorDialog.GetPath(), wxBITMAP_TYPE_ANY);
        fileInEditor = imageFileEditorDialog.GetPath();
        fileInEditorExt = fileInEditor.GetExt().Lower();

        originalBitmap = imageBitmap;
        originalImageSize = imageBitmap.GetSize();

        scaleImageToEditor(imageBitmap.GetWidth(), imageBitmap.GetHeight());
        imageClientDC->DrawBitmap(imageBitmap, 0, 0, false);
        rotationState = 0;

        saveImageFromEditorBtn->Enable();
        rotateImageBtn->Enable();
        clearEditorBtn->Enable();
    }
}
//metoda zapisujaca edytowane zdjecie na dysku
void imageDialog::saveImage(wxCommandEvent& WXUNUSED(event)) {

    wxFileDialog imageSaveDialog(this, wxT("Save file"), "", "", (fileInEditorExt.Upper() + " files|*." + fileInEditorExt), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition, wxDefaultSize);

    if (imageSaveDialog.ShowModal() == wxID_OK) {
        if (fileInEditorExt == wxT("png")) {
            originalBitmap.SaveFile(imageSaveDialog.GetPath(), wxBITMAP_TYPE_PNG);
        }
        else if (fileInEditorExt == wxT("jpg") || fileInEditorExt == wxT("jpeg")) {
            originalBitmap.SaveFile(imageSaveDialog.GetPath(), wxBITMAP_TYPE_JPEG);
        }
        else if (fileInEditorExt == wxT("gif")) {
            originalBitmap.SaveFile(imageSaveDialog.GetPath(), wxBITMAP_TYPE_GIF);
        }
    }
}
//metoda obracajaca zdjecie
void imageDialog::rotate(wxCommandEvent& WXUNUSED(event)) {

    wxSize viewerSize(imageBitmap.GetWidth(), imageBitmap.GetHeight());
    wxBitmap copiedViewerBitmap(imageBitmap);
    memoryDC->SelectObject(copiedViewerBitmap);
    memoryDC->Blit(wxPoint(0, 0), viewerSize, imageClientDC, wxPoint(-1, -1), wxCOPY, true, wxDefaultPosition);

    //wxImage tmpImageInViewer = imageBitmap.ConvertToImage();
    wxImage tmpImageInViewer = copiedViewerBitmap.ConvertToImage();
    tmpImageInViewer = tmpImageInViewer.Rotate90(true);

    wxImage tmpOriginal = originalBitmap.ConvertToImage();
    tmpOriginal = tmpOriginal.Rotate90(true);

    if (rotationState == 3) {
        rotationState = 0;
    }
    else {
        rotationState++;
    }

    wxBitmap rotatedBitmap(tmpImageInViewer);
    wxBitmap rotatedOriginal(tmpOriginal);
    rotatedImageSize = rotatedBitmap.GetSize();
    imageBitmap = rotatedBitmap;
    originalBitmap = rotatedOriginal;

    imageClientDC->Clear();
    scaleImageToEditor(imageBitmap.GetWidth(), imageBitmap.GetHeight());
    imageClientDC->DrawBitmap(imageBitmap, 0, 0, false);
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
        heightScale = ((float)panelH - 166) / (float)imageH;
        heightScaleValue = heightScale;
        widthScale = ((float)panelW - 120) / (float)imageW; 
        widthScaleValue = widthScale;

        rotatedHeightScale = ((float)panelH - 166) / (float)originalBitmap.GetHeight();
        rotatedWidthScale = ((float)panelW - 120) / (float)originalBitmap.GetWidth();
    }

    //viewerClientDC->SetUserScale(widthScale, heightScale);
    wxImage tmpImg = imageBitmap.ConvertToImage();
    imageBitmap = wxBitmap(tmpImg.Scale(imageBitmap.GetWidth() * widthScale, imageBitmap.GetHeight() * heightScale), wxIMAGE_QUALITY_HIGH);
}
//metoda czyszczaca edytor
void imageDialog::clearEditor(wxCommandEvent& WXUNUSED(event)) {
    imageClientDC->Clear();
    memoryDC->Clear();
    saveImageFromEditorBtn->Disable();
    rotateImageBtn->Disable();
    clearEditorBtn->Disable();
}
void imageDialog::onMouseLeftMove(wxMouseEvent& event) {
    if (isPressed && event.Dragging()) {
        wxPoint movingMousePosition = event.GetPosition();
        memoryDC->SelectObject(originalBitmap);
        
        wxPen pen;
        pen.SetWidth(10);
        pen.SetColour(*wxBLACK);
        memoryDC->SetPen(pen);
        imageClientDC->SetPen(*wxBLACK);
        imageClientDC->DrawLine(movingMousePosition, mousePosition);

        int panelWidth = 0;
        int panelHeight = 0;
        GetSize(&panelWidth, &panelHeight);
        
        if (rotationState == 0 || rotationState == 2) {
            memoryDC->DrawLine(movingMousePosition.x / widthScaleValue, movingMousePosition.y / heightScaleValue, mousePosition.x / widthScaleValue, mousePosition.y / heightScaleValue);
        }
        else {
            memoryDC->DrawLine(movingMousePosition.x / rotatedWidthScale, movingMousePosition.y / rotatedHeightScale, mousePosition.x / rotatedWidthScale, mousePosition.y / rotatedHeightScale);
        }
       
        imageClientDC->SetPen(wxNullPen);
        memoryDC->SetPen(wxNullPen);
        mousePosition = movingMousePosition;
    }
}
void imageDialog::onMouseLeftPressed(wxMouseEvent& event) {
    isPressed = true;
    mousePosition = event.GetPosition();
}
void imageDialog::onMouseLeftUp(wxMouseEvent& event) {
    isPressed = false;
    //scaleImageToEditor(imageBitmap.GetWidth(), imageBitmap.GetHeight());
}