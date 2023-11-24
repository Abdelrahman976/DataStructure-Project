#ifndef TEditor
#define TEditor
#include <iostream>
#include <string>
#include <fstream>
#include "SuffixTree.h"

using namespace std;

class TextEditor {
public:
    TextEditor();
    void menu();
private:
    bool initialState{false};
    string text;
    string currentText;
    string clipboard;
    string previousText;
    SuffixTree myTree;
    void saveState();
    void insert(const string& str);
    void deleteSubstring(int start, int length);
    bool find(const string& search);
    void undo();
    void redo();
    void copy(int start, int length);
    void paste(int position);
    void cut(int start, int length);
    void displayContent();
    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);
};

#endif
