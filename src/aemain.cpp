////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/aui/auibook.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>

#include <cstdio>

////////////////////////////////////////////////////////////////////////////////

class aeMainFrame : public wxFrame {
public:
    aeMainFrame();
    ~aeMainFrame();

    bool Create(
        const wxPoint &pos=wxDefaultPosition,
        const wxSize &size=wxDefaultSize,
        long style=wxDEFAULT_FRAME_STYLE
    );

protected:
    enum {
        ID_ButtonNew
    };

    bool CreateRibbonBar();
    bool CreateNotebook();
    bool CreateStatusBar();

    void OnNewDocument(wxRibbonButtonBarEvent &event);

private:
    wxRibbonBar *mRibbonBar;
    wxRibbonPage *mRibbonHome;
    wxRibbonPage *mRibbonMap;
    wxStatusBar *mStatusBar;

    wxAuiNotebook *mNotebook;

    int mTabCount;

    wxDECLARE_EVENT_TABLE();
};

////////////////////////////////////////////////////////////////////////////////

aeMainFrame::aeMainFrame(): mRibbonBar(), mRibbonHome(), mNotebook(), mTabCount() {
}

aeMainFrame::~aeMainFrame() {
}

bool aeMainFrame::Create(const wxPoint &pos, const wxSize &size, long style) {
    if (wxFrame::Create(nullptr, wxID_ANY, _T("aeGIS"), pos, size, style)) {

        CreateRibbonBar();
        CreateNotebook();
        CreateStatusBar();

        wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        SetSizer(sizer);

        sizer->Add(mRibbonBar, 0, wxEXPAND);
        sizer->Add(mNotebook, 1, wxEXPAND);
        sizer->Add(mStatusBar, 0, wxEXPAND);

        wxVersionInfo wxversion = wxGetLibraryVersionInfo();
        wxLogStatus("Using %s", wxversion.GetVersionString());

        return true;
    }

    return false;
}

bool aeMainFrame::CreateRibbonBar() {

    mRibbonBar = new wxRibbonBar(this);

    {
        mRibbonHome = new wxRibbonPage(mRibbonBar, wxID_ANY, _T("Home"));

        wxRibbonPanel *filePanel = new wxRibbonPanel(mRibbonHome, wxID_ANY, _T("File"));
        wxRibbonButtonBar *fileButtons = new wxRibbonButtonBar(filePanel, wxID_ANY);
        fileButtons->AddHybridButton(wxID_NEW, _T("New"), wxArtProvider::GetBitmap(wxART_NEW));
        fileButtons->AddButton(wxID_OPEN, _T("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN));
        fileButtons->AddButton(wxID_SAVE, _T("Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE));
        fileButtons->AddButton(wxID_SAVEAS, _T("Save As"), wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS));

        wxRibbonPanel *clipboardPanel = new wxRibbonPanel(mRibbonHome, wxID_ANY, _T("Clipboard"));
        wxRibbonButtonBar *clipboardButtons = new wxRibbonButtonBar(clipboardPanel, wxID_ANY);
        clipboardButtons->AddHybridButton(wxID_PASTE, _T("Paste"), wxArtProvider::GetBitmap(wxART_PASTE));
        clipboardButtons->AddButton(wxID_CUT, _T("Cut"), wxArtProvider::GetBitmap(wxART_CUT));
        clipboardButtons->AddHybridButton(wxID_COPY, _T("Copy"), wxArtProvider::GetBitmap(wxART_COPY));
    }

    {
        mRibbonMap = new wxRibbonPage(mRibbonBar, wxID_ANY, _T("Map"));

        wxRibbonPanel *dataPanel = new wxRibbonPanel(mRibbonMap, wxID_ANY, _T("Data"));
        wxRibbonButtonBar *dataButtons = new wxRibbonButtonBar(dataPanel, wxID_ANY);
        dataButtons->AddHybridButton(wxID_ANY, _T("Add"), wxArtProvider::GetBitmap(wxART_INFORMATION));
    }

    //! @todo create more ribbon controls

    mRibbonBar->ShowPanels(wxRIBBON_BAR_MINIMIZED);
    mRibbonBar->Realize();

    return true;
}

bool aeMainFrame::CreateNotebook() {
    mNotebook = new wxAuiNotebook(this);

    return true;
}

bool aeMainFrame::CreateStatusBar() {
    mStatusBar = new wxStatusBar(this);

    const int fields = 1;
    int widths[fields] = { -1 };
    int styles[fields] = { wxSB_NORMAL };

    mStatusBar->SetFieldsCount(fields, widths);
    mStatusBar->SetStatusStyles(fields, styles);

    return true;
}

void aeMainFrame::OnNewDocument(wxRibbonButtonBarEvent & /*event*/) {
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

DECLARE_APP(aeApp)

////////////////////////////////////////////////////////////////////////////////

aeApp::aeApp(): mMainFrame() {
}

bool aeApp::OnInit() {
    if (wxApp::OnInit()) {
        if (!mMainFrame) {
            mMainFrame = new aeMainFrame();
            mMainFrame->Create(wxDefaultPosition, wxSize(640, 480));
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

IMPLEMENT_APP(aeApp)

////////////////////////////////////////////////////////////////////////////////
//  EOF
////////////////////////////////////////////////////////////////////////////////

