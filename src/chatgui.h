#ifndef CHATGUI_H_
#define CHATGUI_H_

#include <wx/wx.h>
#include <mutex>
#include <memory>

//#define __GXX_ABI_VERSION 1011
class ChatLogic; // forward declaration

// middle part of the window containing the dialog between user and chatbot
class ChatBotPanelDialog : public wxScrolledWindow
{
private:
    // control elements
    wxBoxSizer *_dialogSizer;
    wxBitmap _image;

    ChatLogic *_chatLogic;

public:
    // constructor / destructor
    ChatBotPanelDialog(wxWindow *parent, wxWindowID id);
    ~ChatBotPanelDialog();

    // getter / setter
    ChatLogic *GetChatLogicHandle() { return _chatLogic; }

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    // proprietary functions
    void AddDialogItem(wxString text, bool isFromUser = true);
    void PrintChatbotResponse(std::string response);

    DECLARE_EVENT_TABLE()
};

// dialog item shown in ChatBotPanelDialog
class ChatBotPanelDialogItem : public wxPanel
{
private:
    // control elements
    wxStaticBitmap *_chatBotImg;
    wxStaticText *_chatBotTxt;

public:
    // constructor / destructor
    ChatBotPanelDialogItem(wxPanel *parent, wxString text, bool isFromUser);
};

// frame containing all control elements
class ChatBotFrame : public wxFrame
{
private:
    // control elements
    std::unique_ptr<ChatBotPanelDialog> _panelDialog;
    std::unique_ptr<wxTextCtrl> _userTextCtrl, _userTextCtrl2;
    wxString userText;
    std::vector<std::string> values = {"Greetings","Travel","Sport","Technology"};
    std::mutex mtx;

    // events
    void OnEnter(wxCommandEvent &WXUNUSED(event));
    void OnWrong(wxCommandEvent &WXUNUSED(event));
public:
    // constructor / desctructor
    ChatBotFrame(const wxString &title);
};

// control panel for background image display
class ChatBotFrameImagePanel : public wxPanel
{
    // control elements
    wxBitmap _image;

public:
    // constructor / desctructor
    ChatBotFrameImagePanel(wxFrame *parent);

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    DECLARE_EVENT_TABLE()
};

// wxWidgets app that hides main()
class ChatBotApp : public wxApp
{
public:
    // events
    virtual bool OnInit();
};

#endif /* CHATGUI_H_ */
