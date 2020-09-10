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

### How to compile and run?
You need only to run CMake and run the generated executable file.