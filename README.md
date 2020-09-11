# GermanChatBot
### A chatbot for german language

In this project, I have implemented a chatbot in which the topics of 
the user input is identified. For now, it is only working for German
language because of the training data. So, with the suitable training
data, it will work for any other languages. Also, since the standard 
C++ string class only supports ascii characters, the german sentences
should be entered by english format which means ä, ö, ü, and ß should
be replaced with ae, oe, ue, ss.


There are different approaches toward the recognition as follows,
1. Search in a list of known important German words
2. Sentence classification based on Fasttext library
2. Removing the stopwords

There is a simple training set under data/train.csv that users can replace
it with whatver they need to.

## An example of chat
![chat](screenshot.png)

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

- A version of [fasttext library](https://github.com/facebookresearch/fastText) is already copied inside the source directory.
So there is no need for installation. 

## Basic Build Instructions

1. git clone https://github.com/rostam/GermanChatBot
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./GermanChatBot`.


## Project Rubrics
### A variety of control structures are used in the project.
Different control structures can be found in [the functions eraseSubStrings, handle, and the constructure HandleText::HandleText() ](https://github.com/rostam/GermanChatBot/blob/master/src/HandleText.cpp)

### The project reads data from an external file or writes data to a file as part of the necessary operation of the program.
Three files are read in the project as follows:
```c++
    std::ifstream in("GermanStopWords.txt");
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

    std::ifstream in3("data/classes.csv");
    int cnt = 1;
    while (std::getline(in3, line))
    {
        labels_classes[std::string("__label__") + std::to_string(cnt)] = line;
        cnt++;
    }

//    LanguageIdentificationFT.loadModel("lid.176.bin");
    fasttext::Args a = fasttext::Args();
    a.parseArgs({"fasttext","supervised", "-input", "data/train.csv", "-output", "model_cooking"});
    SentenceClassificationFT.train(a);
```
