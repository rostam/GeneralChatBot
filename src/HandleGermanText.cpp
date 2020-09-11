//
// Created by rostam on 11.09.20.
//

#include "HandleGermanText.h"
#include <sstream>

HandleGermanText::HandleGermanText() : HandleText("data/train.csv") {
    std::ifstream in("data/GermanStopWords.txt");
    std::string line;
    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        std::string a;
        if (!(iss >> a)) { break; }
        stopwords.insert(a);
    }

    std::ifstream in2("data/GermanNounsProper.csv");
    while (std::getline(in2, line))
    {
        int pos = line.find(',');
        nouns[line.substr(0,pos)] = line.substr(pos + 1) ;
    }
}

/**
 * Removes language specific stop words
 * @param input the user input
 * @return the input after the stop words are removed
 */
std::string HandleGermanText::RemoveStopWords(const std::string& input) {
    return EraseSubStrings(input, stopwords);
}

/**
 * Recognize language specific nouns
 * @param input the usre input
 * @return
 */
std::vector<std::string> HandleGermanText::RecognizeNouns(const std::string &input) {
    std::istringstream iss(RemoveStopWords(input));
    std::string res;
    std::string a;
    std::string output;
    std::vector<std::string> ret;
    while (iss >> a) {
        if(nouns.find(a) != nouns.end()) {
            std::string inf = nouns[a];
            if(inf != "OTHERS") ret.push_back(a + " (" + inf+ ")");
        }
    }
    return ret;
}
