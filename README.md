# GeneralChatBot
## A Multilingual Chatbot Framework

Welcome to the GeneralChatBot project! This is a multilingual chatbot framework that identifies the topics of user input. Currently, it supports the German language due to the available training data. However, with the right training data, it can be extended to support any language.

Please note that as the standard C++ string class only supports ASCII characters, German sentences should be entered in English format. This means special characters like ä, ö, ü, and ß should be replaced with ae, oe, ue, ss respectively.

### Recognition Approaches
The chatbot uses several approaches to recognize and process user input:

1. Searching in a list of known important German words.
2. Sentence classification based on the Fasttext library.
3. Removing the stopwords.
4. Retraining based on the user input.

### Training Data
A simple training set is provided under `data/train.csv`. Users can replace this with their own data as needed. The chatbot also allows users to retrain a specific sentence by clicking the 'wrong' button and entering the new class.

![Chat Example](screenshot.png)

## Local Setup
### Dependencies
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

A version of the [fasttext library](https://github.com/facebookresearch/fastText) is already included in the source directory, so there's no need for separate installation. 

### Build Instructions
1. Clone the repository: `git clone https://github.com/rostam/GeneralChatBot`
2. Create a build directory in the top level directory: `mkdir build && cd build`
3. Compile the project: `cmake .. && make`
4. Run the chatbot: `./GeneralChatBot`.

## Project Rubrics
The project adheres to a variety of rubrics including the use of control structures, reading and writing data from external files, user input handling, class organization, inheritance, multithreading, and more. Detailed examples and explanations can be found in the source code files like `HandleText.cpp`, `HandleGermanText.cpp`, and `chatgui.cpp`.

For more information, please refer to the source code and comments therein. Contributions to extend the functionality of this chatbot, especially for supporting more languages, are welcome!

#### The project reads data from an external file or writes data to a file as part of the necessary operation of the program.
#### The project accepts input from a user as part of the necessary operation of the program.

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
#### The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
#### All class data members are explicitly specified as public, protected, or private.
#### All class members that are set to argument values are initialized through member initialization lists.
#### All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.
#### Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.
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

#### Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.
#### One function is overloaded with different signatures for the same function name.
#### One member function in an inherited class overrides a virtual base class member function.
#### One function is declared with a template that allows it to accept a generic parameter.

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

#### At least two variables are defined as references, or two functions use pass-by-reference in the project code.
#### At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor.
#### The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.
#### For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined.
#### For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object.
#### The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.
These features can be found in different files for example in [chatgui.cpp](src/chatgui.cpp), [HandleGermanText.cpp](src/HandleGermanText.cpp),
or [HandleText.cpp](src/HandleText.cpp)


#### The project uses multiple threads in the execution.
#### A promise and future is used to pass data from a worker thread to a parent thread in the project code.
#### A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code.
#### A std::condition_variable is used in the project code to synchronize thread execution.
These features are used in [chatgui.cpp](src/chatgui.cpp):
```c++
void ChatBotFrame::OnWrong(wxCommandEvent &WXUNUSED(event)) {
    std::promise<std::string> prom;
    std::future<std::string> fut = prom.get_future();

    if(std::string(userText.mb_str()) == "") return;
    auto dlg = new wxTextEntryDialog(this,
                                     "What is the correct class (Greetings,Travel,Sport,Technology):",
                                     "Improve Training", "Travel");
    dlg->ShowModal();
    wxString val = dlg->GetValue();

    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    bool writeToFile = false;
    auto CheckTraingingValueInTrainingFile = [&ready,&writeToFile](std::string val) -> bool {
        std::ifstream in("data/update_train.csv");
        std::string line;
        while (std::getline(in, line))
        {
            std::istringstream iss(line);
            std::string a;
            if (!(iss >> a)) {
                break;
            }
            if(a == val) {
                ready = true;
                writeToFile = false;
                return false;
            }
        }
        writeToFile = true;
        ready = true;
        return true;
    };

    auto AppendTraningValueToTrainingFile = [&cv,&ready,&m,&writeToFile](std::future<std::string>& val) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&ready]{return ready;});
        if(writeToFile) {
            std::ofstream out("data/update_train.csv", std::ofstream::out | std::ofstream::app);
            out << val.get() << std::endl;
            out.close();
        }
        lk.unlock();
        cv.notify_one();
    };

//    CheckTraingingValueInTrainingFile(std::string(userText.mb_str()) + "," + std::string(val.mb_str()));
    {
        std::thread thread_object(CheckTraingingValueInTrainingFile, std::string(userText.mb_str()) + "," + std::string(val.mb_str()));
        thread_object.join();
    }


    {
        std::unique_lock<std::mutex> lck(mtx);
        prom.set_value (  std::string(userText.mb_str()) + "," + std::string(val.mb_str()));
        std::thread thread_object(AppendTraningValueToTrainingFile, std::ref(fut));
        thread_object.join();
    }
}

```
