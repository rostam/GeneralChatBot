//
// Created by rostam on 11.09.20.
//

#ifndef GENERALCHATBOT_HANDLEGERMANTEXT_H
#define GENERALCHATBOT_HANDLEGERMANTEXT_H

#include "HandleText.h"


class HandleGermanText : public  HandleText {
    std::unordered_set<std::string> stopwords;
    std::unordered_map<std::string,std::string> nouns;
protected:
    std::string RemoveStopWords(const std::string& input) override;
    std::vector<std::string> RecognizeNouns(const std::string& input) override;
public:
    HandleGermanText();
};


#endif //GENERALCHATBOT_HANDLEGERMANTEXT_H
