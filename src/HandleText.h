//
// Created by rostam on 09.09.20.
//

#ifndef GERMANCHATBOT_HANDLETEXT_H
#define GERMANCHATBOT_HANDLETEXT_H

#include <unordered_set>
#include <unordered_map>

class HandleText {
    std::string eraseSubStrings(const std::string& mainStr, const std::unordered_set<std::string> & strList);
    std::unordered_set<std::string> stopwords;
    std::unordered_map<std::string,std::string> nouns;

public:
    HandleText();
    std::vector<std::string> handle(std::string input);
};


#endif //GERMANCHATBOT_HANDLETEXT_H
