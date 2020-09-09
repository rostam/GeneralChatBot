//
// Created by rostam on 09.09.20.
//

#include "HandleText.h"
#include <sstream>
#include <vector>
#include <fstream>


HandleText::HandleText() {
    std::ifstream in("GermanStopWords.txt");
    std::string line;
    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        std::string a;
        if (!(iss >> a)) { break; } // error

        // process pair (a,b)
        stopwords.insert(a);
    }

    std::ifstream in2("GermanNouns.csv");
    while (std::getline(in2, line))
    {
        int pos = line.find(',');
        nouns[line.substr(0,pos)] = line.substr(pos, 30) ;
    }

    LanguageIdentificationFT.loadModel("/home/rostam/Downloads/lid.176.bin");
}

/*
 * Erase all Occurrences of all given substrings from main string using C++11 stuff
 */
std::string HandleText::eraseSubStrings(const std::string& mainStr, const std::unordered_set<std::string> & strList)
{
    std::istringstream iss(mainStr);
    std::string res;
    std::string a;
    while (iss >> a) {
        if(strList.find(a) == strList.end())
            res += a + " ";
    }
    return res;
}


std::vector<std::string> HandleText::handle(std::string input) {
    std::vector<std::string> ret;
    std::string messageStopWordsRemoved = eraseSubStrings(input, stopwords);
//    _chatLogic->SendMessageToUser("The domain of your sentence is: ");
//    _chatLogic->SendMessageToUser(messageStopWordsRemoved);

    std::istringstream myiss(input);
    std::vector<std::pair<fasttext::real, std::string>> predictions;
    LanguageIdentificationFT.predictLine(myiss, predictions, 2, 0.1);
    if(!predictions.empty()) {
        std::string predicted = predictions[0].second;
        std::string rett;
        if(predicted.find("de") != std::string::npos) rett = "German";
        else if(predicted.find("en") !=std::string::npos || predicted.find("ca") !=std::string::npos) rett = "English";
        else if(predicted.find("es") !=std::string::npos) rett = "Spanish";
        else rett = "the languages other than German, English, and Spanish.";
        ret.push_back(std::string("You entered your message in ") + rett);
    }
//    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//    std::string MessageStopWordsRemovedStemmed  = converter.to_bytes(Cistem::stem(converter.from_bytes(messageStopWordsRemoved)));
//    _chatLogic->SendMessageToUser(MessageStopWordsRemovedStemmed);

    std::istringstream iss(messageStopWordsRemoved);
    std::string res;
    std::string a;
    std::string output;
    while (iss >> a) {
        output += a + " ";
        if(nouns.find(a) != nouns.end()) {
            std::string inf = nouns[a];
            if(inf.find("Toponym") != std::string::npos) output += "(LOCATION) ";
            else if(inf.find("Nachname") != std::string::npos) output += "(PERSON) ";
            else if(inf.find("Vorname") != std::string::npos) output += "(PERSON) ";
        }
    }

    return ret;
}
