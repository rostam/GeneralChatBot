# GeneralChatBot
### A chatbot for different languages

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
Different control structures can be found in [the functions EraseSubStrings, handle, and the constructure HandleText::HandleText() ](https://github.com/rostam/GermanChatBot/blob/master/src/HandleText.cpp)

### The project reads data from an external file or writes data to a file as part of the necessary operation of the program.
### The project accepts input from a user as part of the necessary operation of the program.

Three files are read in the project as follows in [HandleGermanText.cpp](src/HandleGermanText.cpp):
```c++
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
```
### The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
### All class data members are explicitly specified as public, protected, or private.
### All class members that are set to argument values are initialized through member initialization lists.
### All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.
### Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.
```c++
//**
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
```

### Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.
### One function is overloaded with different signatures for the same function name.
### One member function in an inherited class overrides a virtual base class member function.
### One function is declared with a template that allows it to accept a generic parameter.

```c++
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

```

### At least two variables are defined as references, or two functions use pass-by-reference in the project code.
### At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor.
### The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.
### For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined.
### For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object.
### The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.


### The project uses multiple threads in the execution.
### A promise and future is used to pass data from a worker thread to a parent thread in the project code.
### A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code.
### A std::condition_variable is used in the project code to synchronize thread execution.

