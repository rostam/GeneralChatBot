#include <wx/filename.h>
#include <wx/colour.h>
#include <wx/image.h>
#include <string>
#include <thread>
#include <future>
#include "chatbot.h"
#include "chatlogic.h"
#include "chatgui.h"
#include <sstream>

// size of chatbot window
const int width = 414;
const int height = 736;

// wxWidgets APP
IMPLEMENT_APP(ChatBotApp);

std::string dataPath = "../";
std::string imgBasePath = dataPath + "images/";

bool ChatBotApp::OnInit()
{
    // create window with name and show it
    ChatBotFrame *chatBotFrame = new ChatBotFrame(wxT("ChatBot"));
    chatBotFrame->Show(true);

    return true;
}

// wxWidgets FRAME
ChatBotFrame::ChatBotFrame(const wxString &title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(width, height))
{
    // create panel with background image
    ChatBotFrameImagePanel *ctrlPanel = new ChatBotFrameImagePanel(this);

    // create controls and assign them to control panel
    _panelDialog = std::make_unique<ChatBotPanelDialog>(ctrlPanel, wxID_ANY);


    // create text control for user input
    int idTextXtrl = 1;
    _userTextCtrl = std::make_unique<wxTextCtrl>(ctrlPanel, idTextXtrl, "", wxDefaultPosition, wxSize(width, 50), wxTE_PROCESS_ENTER, wxDefaultValidator, wxTextCtrlNameStr);
    Connect(idTextXtrl, wxEVT_TEXT_ENTER, wxCommandEventHandler(ChatBotFrame::OnEnter));
    _userTextCtrl->SetFont(wxFont( 16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL ,wxFONTWEIGHT_NORMAL));


    int idTextXtrl2 = 5;
    _userTextCtrl2 = std::make_unique<wxTextCtrl>(ctrlPanel, idTextXtrl2, "ChatBot", wxDefaultPosition, wxSize(width, 50), wxTE_RICH2||wxTE_MULTILINE, wxDefaultValidator, wxTextCtrlNameStr);
//    _userTextCtrl2->SetDefaultStyle(wxTextAttr(*wxRED));
//    _userTextCtrl2->AppendText("ChatBot");
    _userTextCtrl2->SetFont(wxFont( 30,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL ,wxFONTWEIGHT_BOLD));
    _userTextCtrl2->SetBackgroundColour(*wxLIGHT_GREY);
    _userTextCtrl2->SetEditable(false);

    auto buttonpanels = new wxPanel(ctrlPanel, 6);
    auto t1 = new wxButton(buttonpanels, 6,"Correct");
    auto t2 = new wxButton(buttonpanels, 7,"Wrong");
    wxBoxSizer *horBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    horBoxSizer->Add(t1,1,wxEXPAND | wxALL);
    horBoxSizer->Add(t2,1,wxEXPAND | wxALL);
    buttonpanels->SetSizer(horBoxSizer);
    Connect(7, wxEVT_BUTTON, wxCommandEventHandler(ChatBotFrame::OnWrong));
    // create vertical sizer for panel alignment and add panels
    wxBoxSizer *vertBoxSizer = new wxBoxSizer(wxVERTICAL);
//    vertBoxSizer->AddSpacer(90);
    vertBoxSizer->Add(_userTextCtrl2.get(), 1, wxEXPAND | wxALL,5);
    vertBoxSizer->Add(_panelDialog.get(), 6, wxEXPAND | wxALL, 0);
    vertBoxSizer->Add(_userTextCtrl.get(), 1, wxEXPAND | wxALL, 5);
    vertBoxSizer->Add(buttonpanels, 1, wxEXPAND | wxALL, 5);
    ctrlPanel->SetSizer(vertBoxSizer);

    // position window in screen center
    this->Centre();
}

void ChatBotFrame::OnWrong(wxCommandEvent &WXUNUSED(event)) {
    std::promise<std::string> prom;
    std::future<std::string> fut = prom.get_future();

    if(std::string(userText.mb_str()) == "") return;
    auto dlg = new wxTextEntryDialog(this,
                                     "What is the correct class (Greetings,Travel,Sport,Technology):",
                                     "Improve Training", "Travel");
    dlg->ShowModal();
    wxString val = dlg->GetValue();

    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    bool writeToFile = false;
    auto CheckTraingingValueInTrainingFile = [&ready,&writeToFile](std::string val) -> bool {
        std::ifstream in("data/update_train.csv");
        std::string line;
        while (std::getline(in, line))
        {
            std::istringstream iss(line);
            std::string a;
            if (!(iss >> a)) {
                break;
            }
            if(a == val) {
                ready = true;
                writeToFile = false;
                return false;
            }
        }
        writeToFile = true;
        ready = true;
        return true;
    };

    auto AppendTraningValueToTrainingFile = [&cv,&ready,&m,&writeToFile](std::future<std::string>& val) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&ready]{return ready;});
        if(writeToFile) {
            std::ofstream out("data/update_train.csv", std::ofstream::out | std::ofstream::app);
            out << val.get() << std::endl;
            out.close();
        }
        lk.unlock();
        cv.notify_one();
    };

//    CheckTraingingValueInTrainingFile(std::string(userText.mb_str()) + "," + std::string(val.mb_str()));
    {
        std::thread thread_object(CheckTraingingValueInTrainingFile, std::string(userText.mb_str()) + "," + std::string(val.mb_str()));
        thread_object.join();
    }


    {
        std::unique_lock<std::mutex> lck(mtx);
        prom.set_value (  std::string(userText.mb_str()) + "," + std::string(val.mb_str()));
        std::thread thread_object(AppendTraningValueToTrainingFile, std::ref(fut));
        thread_object.join();
    }
}

void ChatBotFrame::OnEnter(wxCommandEvent &WXUNUSED(event))
{
    // retrieve text from text control
    userText = _userTextCtrl->GetLineText(0);

    // add new user text to dialog
    _panelDialog->AddDialogItem(userText, true);

    // delete text in text control
    _userTextCtrl->Clear();

    // send user text to chatbot 
     _panelDialog->GetChatLogicHandle()->SendMessageToChatbot(std::string(userText.mb_str()));
}

BEGIN_EVENT_TABLE(ChatBotFrameImagePanel, wxPanel)
EVT_PAINT(ChatBotFrameImagePanel::paintEvent) // catch paint events
END_EVENT_TABLE()

ChatBotFrameImagePanel::ChatBotFrameImagePanel(wxFrame *parent) : wxPanel(parent)
{
}

void ChatBotFrameImagePanel::paintEvent(wxPaintEvent &evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void ChatBotFrameImagePanel::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void ChatBotFrameImagePanel::render(wxDC &dc)
{
    // rescale image to fit window dimensions
    wxSize sz = this->GetSize();
}

BEGIN_EVENT_TABLE(ChatBotPanelDialog, wxPanel)
EVT_PAINT(ChatBotPanelDialog::paintEvent) // catch paint events
END_EVENT_TABLE()

ChatBotPanelDialog::ChatBotPanelDialog(wxWindow *parent, wxWindowID id)
    : wxScrolledWindow(parent, id)
{
    // sizer will take care of determining the needed scroll size
    _dialogSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(_dialogSizer);

    // allow for PNG images to be handled
    wxInitAllImageHandlers();

    // create chat logic instance
    _chatLogic = new ChatLogic(); 

    // pass pointer to chatbot dialog so answers can be displayed in GUI
    _chatLogic->SetPanelDialogHandle(this);
}

ChatBotPanelDialog::~ChatBotPanelDialog()
{
    delete _chatLogic;
}

void ChatBotPanelDialog::AddDialogItem(wxString text, bool isFromUser)
{
    // add a single dialog element to the sizer
    ChatBotPanelDialogItem *item = new ChatBotPanelDialogItem(this, text, isFromUser);
    _dialogSizer->Add(item, 0, wxALL | (isFromUser == true ? wxALIGN_LEFT : wxALIGN_RIGHT), 8);
    _dialogSizer->Layout();

    // make scrollbar show up
    this->FitInside(); // ask the sizer about the needed size
    this->SetScrollRate(5, 5);
    this->Layout();

    // scroll to bottom to show newest element
    int dx, dy;
    this->GetScrollPixelsPerUnit(&dx, &dy);
    int sy = dy * this->GetScrollLines(wxVERTICAL);
    this->DoScroll(0, sy);
}

void ChatBotPanelDialog::PrintChatbotResponse(std::string response)
{
    // convert string into wxString and add dialog element
    wxString botText(response.c_str(), wxConvUTF8);
    AddDialogItem(botText, false);
}

void ChatBotPanelDialog::paintEvent(wxPaintEvent &evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void ChatBotPanelDialog::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void ChatBotPanelDialog::render(wxDC &dc)
{
    wxSize sz = this->GetSize();
}

ChatBotPanelDialogItem::ChatBotPanelDialogItem(wxPanel *parent, wxString text, bool isFromUser)
    : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_NONE)
{
    // retrieve image from chatbot
    _chatBotTxt = new wxStaticText(this, wxID_ANY, text, wxPoint(-1, -1), wxSize(150, -1), wxALIGN_CENTRE | wxBORDER_NONE);
    _chatBotTxt->SetForegroundColour(isFromUser == true ? wxColor(*wxBLACK) : wxColor(*wxBLACK));

    // create sizer and add elements
    wxBoxSizer *horzBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    horzBoxSizer->Add(_chatBotTxt, 8, wxEXPAND | wxALL, 1);
//    horzBoxSizer->Add(_chatBotImg, 2, wxEXPAND | wxALL, 1);
    this->SetSizer(horzBoxSizer);

    // wrap text after 150 pixels
    _chatBotTxt->Wrap(150);
}
