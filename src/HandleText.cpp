//
// Created by rostam on 09.09.20.
//

#include "HandleText.h"
#include <sstream>
#include <vector>
#include <fstream>

void HandleText::train(const std::vector<std::string> args) {
    fasttext::Args a = fasttext::Args();
    a.parseArgs(args);
    std::shared_ptr<fasttext::FastText> fasttext = std::make_shared<fasttext::FastText>();
    std::string outputFileName;

    if (a.hasAutotune() &&
        a.getAutotuneModelSize() != fasttext::Args::kUnlimitedModelSize) {
        outputFileName = a.output + ".ftz";
    } else {
        outputFileName = a.output + ".bin";
    }
    std::ofstream ofs(outputFileName);
    if (!ofs.is_open()) {
        throw std::invalid_argument(
                outputFileName + " cannot be opened for saving.");
    }
    ofs.close();
//    if (a.hasAutotune()) {
//        Autotune autotune(fasttext);
//        autotune.train(a);
//    } else {
        fasttext->train(a);
//    }
//    fasttext->saveModel(outputFileName);
    fasttext->saveVectors(a.output + ".vec");
    if (a.saveOutput) {
        fasttext->saveOutput(a.output + ".output");
    }
}



HandleText::HandleText(const std::string& TrainingFile) {

    std::ifstream in3("data/classes.csv");
    std::string line;
    int cnt = 1;
    while (std::getline(in3, line))
    {
        labels_classes[std::string("__label__") + std::to_string(cnt)] = line;
        cnt++;
    }

//    LanguageIdentificationFT.loadModel("lid.176.bin");
    fasttext::Args a = fasttext::Args();
    a.parseArgs({"fasttext","supervised", "-input", TrainingFile, "-output", "model_cooking"});
    SentenceClassificationFT.train(a);
//    SentenceClassificationFT.saveModel("test.bin");
//    SentenceClassificationFT.loadModel("test.bin");
}

/*
 * Erase all Occurrences of all given substrings from main string using C++11 stuff
 */
std::string HandleText::EraseSubStrings(const std::string& mainStr, const std::unordered_set<std::string> & strList)
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
    std::string messageStopWordsRemoved = RemoveStopWords(input);
//    _chatLogic->SendMessageToUser("The domain of your sentence is: ");
//    _chatLogic->SendMessageToUser(messageStopWordsRemoved);

//    std::istringstream myiss(input);
//    std::vector<std::pair<fasttext::real, std::string>> predictions;
//    LanguageIdentificationFT.predictLine(myiss, predictions, 2, 0.1);
//    if(!predictions.empty()) {
//        std::string predicted = predictions[0].second;
//        std::string rett;
//        if(predicted.find("de") != std::string::npos) rett = "German";
//        else if(predicted.find("en") !=std::string::npos || predicted.find("ca") !=std::string::npos) rett = "English";
//        else if(predicted.find("es") !=std::string::npos) rett = "Spanish";
//        else rett = "the languages other than German, English, and Spanish.";
//        ret.push_back(std::string("You entered your message in ") + rett);
//    }

    for(const auto& s : ClassifySentence(input))
        ret.push_back(s);

    for(const auto& s : RecognizeNouns(input))
        ret.push_back(s);

//    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//    std::string MessageStopWordsRemovedStemmed  = converter.to_bytes(Cistem::stem(converter.from_bytes(messageStopWordsRemoved)));
//    _chatLogic->SendMessageToUser(MessageStopWordsRemovedStemmed);


    return ret;
}

std::vector<std::string> HandleText::ClassifySentence(const std::string& input) {
    std::vector<std::pair<fasttext::real, std::string>> predictions2;
    std::istringstream myiss2(input);
    SentenceClassificationFT.predictLine(myiss2, predictions2, 2, 0.1);
    std::vector<std::string> ret;
    if(!predictions2.empty()) {
        ret.push_back(std::string("You are talking about ") + labels_classes[predictions2[0].second]);
    }
    return ret;
}
