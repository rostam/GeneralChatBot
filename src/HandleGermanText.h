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
    /**
     * Removes language specific stop words
     * @param input the user input
     * @return the input after the stop words are removed
     */
    std::string RemoveStopWords(const std::string& input) override;
    /**
     * Recognize language specific nouns
     * @param input the usre input
     * @return
     */
    std::vector<std::string> RecognizeNouns(const std::string& input) override;
public:
    /**
     * Constructor
     */
    HandleGermanText();
};


#endif //GENERALCHATBOT_HANDLEGERMANTEXT_H
