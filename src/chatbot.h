#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "HandleGermanText.h"
#include <memory>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration

class ChatBot
{
private:
    ChatLogic *_chatLogic;

    std::unique_ptr<HandleText> handleText = std::make_unique<HandleGermanText>();;

    // proprietary functions
    int ComputeLevenshteinDistance(std::string s1, std::string s2);

public:
    ChatBot();                     // constructor WITHOUT memory allocation
    ChatBot(std::string filename); // constructor WITH memory allocation
    ~ChatBot();

    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }

    // communication
    void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */