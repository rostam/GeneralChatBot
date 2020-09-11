//
// Created by rostam on 09.09.20.
//

#ifndef GERMANCHATBOT_HANDLETEXT_H
#define GERMANCHATBOT_HANDLETEXT_H

#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "fasttext/fasttext.h"

/**
 * Handling the chatbot input and output strings
 */
class HandleText {
    std::unordered_map<std::string,std::string> labels_classes;
    fasttext::FastText LanguageIdentificationFT;
    fasttext::FastText SentenceClassificationFT;
    void train(const std::vector<std::string> args);
protected:
    /**
     * Erase a lost of substrings from the given string
     * @param mainStr the input string
     * @param strList the list of substrings to be removed
     * @return the input string in which substrings are removed
     */
    std::string EraseSubStrings(const std::string& mainStr, const std::unordered_set<std::string> & strList);
    /**
     * Classify the input sentences based on fasttext
     * @param input the user input
     * @return the different classification
     */
    std::vector<std::string> ClassifySentence(const std::string& input);
    /**
     * Removes language specific stop words
     * @param input the user input
     * @return the input after the stop words are removed
     */
    virtual std::string RemoveStopWords(const std::string& input) = 0;
    /**
     * Recognize language specific nouns
     * @param input the usre input
     * @return
     */
    virtual std::vector<std::string> RecognizeNouns(const std::string& input) = 0;
public:
    /**
     * Constructor of the class
     * @param TrainingFile the input file to be the training dataset for the fasttext model
     */
    explicit HandleText(const std::string& TrainingFile);
    /**
     * Handle the input string from user
     * @param input the user input
     * @return the list of answers to the user
     */
    std::vector<std::string> handle(std::string input);
};


#endif //GERMANCHATBOT_HANDLETEXT_H
