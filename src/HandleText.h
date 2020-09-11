//
// Created by rostam on 09.09.20.
//

#ifndef GERMANCHATBOT_HANDLETEXT_H
#define GERMANCHATBOT_HANDLETEXT_H

#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "fasttext/fasttext.h"

class HandleText {
    std::unordered_map<std::string,std::string> labels_classes;
    fasttext::FastText LanguageIdentificationFT;
    fasttext::FastText SentenceClassificationFT;
    void train(const std::vector<std::string> args);
protected:
    std::string EraseSubStrings(const std::string& mainStr, const std::unordered_set<std::string> & strList);
    std::vector<std::string> ClassifySentence(const std::string& input);
    virtual std::string RemoveStopWords(const std::string& input) = 0;
    virtual std::vector<std::string> RecognizeNouns(const std::string& input) = 0;
public:
    explicit HandleText(const std::string& TrainingFile);
    std::vector<std::string> handle(std::string input);
};


#endif //GERMANCHATBOT_HANDLETEXT_H
