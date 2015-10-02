////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/aui/auibook.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>

////////////////////////////////////////////////////////////////////////////////

class aeMainFrame : public wxFrame {
public:
    aeMainFrame();

    bool Create(
        const wxPoint &pos=wxDefaultPosition,
        const wxSize &size=wxDefaultSize,
        long style=wxDEFAULT_FRAME_STYLE
    );

protected:
    enum {
        ID_ButtonNew
    };

    bool CreateRibbon();
    bool CreateNotebook();

    void OnNewDocument(wxRibbonButtonBarEvent &event);

private:
    wxRibbonBar *mRibbonBar;
    wxRibbonPage *mRibbonHome;
    wxRibbonPage *mRibbonMap;

    wxAuiNotebook *mNotebook;

    int mTabCount;

    wxDECLARE_EVENT_TABLE();
};

////////////////////////////////////////////////////////////////////////////////

aeMainFrame::aeMainFrame(): mRibbonBar(), mRibbonHome(), mNotebook(), mTabCount() {
}

bool aeMainFrame::Create(const wxPoint &pos, const wxSize &size, long style) {
    if (wxFrame::Create(nullptr, wxID_ANY, L"aeGIS", pos, size, style)) {

        CreateRibbon();
        CreateNotebook();

        wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        SetSizer(sizer);

        sizer->Add(mRibbonBar, 0, wxEXPAND);
        sizer->Add(mNotebook, 1, wxEXPAND);

        return true;
    }

    return false;
}

bool aeMainFrame::CreateRibbon() {

    mRibbonBar = new wxRibbonBar(this);

    {
        mRibbonHome = new wxRibbonPage(mRibbonBar, wxID_ANY, L"Home");

        wxRibbonPanel *filePanel = new wxRibbonPanel(mRibbonHome, wxID_ANY, L"File");
        wxRibbonButtonBar *fileButtons = new wxRibbonButtonBar(filePanel, wxID_ANY);
        fileButtons->AddHybridButton(wxID_NEW, L"New", wxArtProvider::GetBitmap(wxART_NEW));
        fileButtons->AddButton(wxID_OPEN, L"Open", wxArtProvider::GetBitmap(wxART_FILE_OPEN));
        fileButtons->AddButton(wxID_SAVE, L"Save", wxArtProvider::GetBitmap(wxART_FILE_SAVE));
        fileButtons->AddButton(wxID_SAVEAS, L"Save As", wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS));

        wxRibbonPanel *clipboardPanel = new wxRibbonPanel(mRibbonHome, wxID_ANY, L"Clipboard");
        wxRibbonButtonBar *clipboardButtons = new wxRibbonButtonBar(clipboardPanel, wxID_ANY);
        clipboardButtons->AddHybridButton(wxID_PASTE, L"Paste", wxArtProvider::GetBitmap(wxART_PASTE));
        clipboardButtons->AddButton(wxID_CUT, L"Cut", wxArtProvider::GetBitmap(wxART_CUT));
        clipboardButtons->AddHybridButton(wxID_COPY, L"Copy", wxArtProvider::GetBitmap(wxART_COPY));
    }

    {
        mRibbonMap = new wxRibbonPage(mRibbonBar, wxID_ANY, L"Map");

        wxRibbonPanel *layerPanel = new wxRibbonPanel(mRibbonMap, wxID_ANY, L"Layer");
        wxRibbonButtonBar *layerButtons = new wxRibbonButtonBar(layerPanel, wxID_ANY);
        layerButtons->AddHybridButton(wxID_ANY, L"Add", wxArtProvider::GetBitmap(wxART_INFORMATION));
    }

    //! @todo create more ribbon controls

    mRibbonBar->Realize();

    return true;
}

bool aeMainFrame::CreateNotebook() {
    mNotebook = new wxAuiNotebook(this);

    return true;
}

void aeMainFrame::OnNewDocument(wxRibbonButtonBarEvent &event) {
    mNotebook->AddPage(new wxRichTextCtrl(mNotebook),
                       wxString::Format(_T("New Document %d"), ++mTabCount));
}

wxBEGIN_EVENT_TABLE(aeMainFrame, wxFrame)
    EVT_RIBBONBUTTONBAR_CLICKED(wxID_NEW, OnNewDocument)
wxEND_EVENT_TABLE()

////////////////////////////////////////////////////////////////////////////////

class aeApp : public wxApp {
public:
    aeApp();

    bool OnInit();
    int OnExit();

private:
    aeMainFrame *mMainFrame;
};

DECLARE_APP(aeApp);

////////////////////////////////////////////////////////////////////////////////

aeApp::aeApp(): mMainFrame() {
}

bool aeApp::OnInit() {
    if (wxApp::OnInit()) {
        if (!mMainFrame) {
            mMainFrame = new aeMainFrame();
            mMainFrame->Create();
        }

        SetTopWindow(mMainFrame);

        //! @todo more initialization

        mMainFrame->Show(true);

        return true;
    }

    return false;
}

int aeApp::OnExit() {
    return wxApp::OnExit();
}

////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(aeApp);

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

