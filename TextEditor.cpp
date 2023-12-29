#include <iostream>
#include <string>
#include <fstream>
#include "TextEditor.h"
#include "SuffixTree.h"

using namespace std;

TextEditor::TextEditor() {
    text = "";
    currentText = "";
}

void TextEditor::insert(const string& str, const int& pos = -1) {
    if(!initialState) {
        initialState = true;
    }
    saveState();
    if (pos == -1)
        text += str;
    else{
        string before = text.substr(0, pos);
        text = before + str + text.substr(pos, text.length() - before.length());
    }
    currentText = text;
}

void TextEditor::deleteSubstring(int start, int length) {
    saveState();
    text.erase(start, length);
    if (text.empty()){
        initialState = false;
    }
    currentText = text;
}

bool TextEditor::find(const string& search, int& ind) {
    SuffixTree myTree;
    myTree.insert(text);
    return myTree.search(search, ind);
}

bool TextEditor::findR(const string& search, const string& replace) {
    saveState();
    string oldText = previousText;
    int ind;
    if(find(search, ind)) {
        deleteSubstring(ind, search.length());
        insert(replace, ind);
        initialState = true;
        previousText = oldText;
        return true;
    }else
        return false;
}

void TextEditor::undo() {
    if(!initialState){
        initialState= true;
        text="";
        cout << "\n****** Nothing to undo ******\n\n";
        return;
    }
    text = previousText;
}

void TextEditor::redo() {
    if (currentText == text) {
        cout << "\n****** Nothing to redo ****** \n";
        return;
    }
    text = currentText;
}

void TextEditor::viewClipboard() {
    if (clipboard.empty())
        cout << "\n****** Clipboard is empty ******\n\n";
    else
        cout << "\n*** Clipboard: " << clipboard << " ***\n";
}

void TextEditor::copy(int start, int length) {
    clipboard = text.substr(start, length);
    viewClipboard();
}

void TextEditor::paste(int position) {
    saveState();
    text.insert(position, clipboard);
    currentText = text;
    cout << "\n\n*** Item Pasted Successfully ***\n\n";
}
void TextEditor::cut(int start, int length) {
    clipboard = text.substr(start, length);
    deleteSubstring(start, length);
    viewClipboard();
}
void TextEditor::displayContent() {
    cout << "Text Editor Content:\n";
    cout << "=======================================================================================================================\n";
    cout << text << endl;
    cout << "=======================================================================================================================\n";
}

void TextEditor::saveToFile(const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << text;
        outFile.close();
        cout << "Text saved to " << filename << endl;
    } else {
        cout << "\n ****** Error: Unable to save the text to " << filename << " ******\n\n";
    }
}

void TextEditor::loadFromFile(const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        getline(inFile, text, '\0');
        currentText = text;
        inFile.close();
        cout << "Text loaded from " << filename << endl;
    } else {
        cout << "\n****** Error: Unable to load text from " << filename << " ******\n\n";
    }
}

void TextEditor::saveState() {
    previousText = text;
}

void TextEditor::menu(){
    while (true) {
        cout << "Text Editor Menu:\n";
        cout << "1. Insert Text\n";
        cout << "2. Delete Text\n";
        cout << "3. Find Text\n";
        cout << "4. Find & Replace Text\n";
        cout << "5. Undo\n";
        cout << "6. Redo\n";
        cout << "7. Cut Text\n";
        cout << "8. Copy Text\n";
        cout << "9. Paste Text\n";
        cout << "10. View Clipboard\n";
        cout << "11. Display Content\n";
        cout << "12. Save to File\n";
        cout << "13. Load from File\n";
        cout << "14. Exit\n";

        cout << "Enter your choice: ";

        string content;

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter text to insert, (<-Back to return to main menu): ";
                cin.ignore();
                getline(cin, content);
                if(content != "<-Back" && content != "<-back") {
                    int pos;
                    cout << "Enter Position (-1: Default insert at End): ";
                    cin >> pos;
                    if (pos == -1)
                        insert(content);
                    else if (pos >= 0)
                        insert(content, pos);
                    else
                        cout << "\n\n****** Invalid position ******\n\n";
                }
                break;
            }
            case 2: {
                if(text.empty()){
                    cout << "\n****** Nothing to delete ******\n\n";
                    break;
                }
                string text;
                cout << "Enter text to delete, (<-Back to return to main menu): ";
                cin.ignore();
                getline(cin, text, '\n');
                if(text != "<-Back" && text != "<-back") {
                    if (findR(text, "")){
                        cout << "\n\n*** Text has been deleted successfully ***\n\n";
                    }else{
                        cout << "\n\n****** Text not found ******\n\n";
                    }
                }
                break;
            }
            case 3: {
                if(text.empty()){
                    cout << "\n****** Nothing to find ******\n\n";
                    break;
                }
                string search;
                cout << "Enter text to find, (<-Back to return to main menu): ";
                cin.ignore();
                getline(cin, search, '\n');
                if(search != "<-Back" && search != "<-back") {
                    int index = 0;
                    bool found = find(search, index);
                    if (found) {
                        cout << "Text found at index: " << index << endl;
                    } else {
                        cout << "\n****** Text not found ******\n\n";
                    }
                }
                break;
            }
            case 4: {
                if(text.empty()){
                    cout << "\n****** Nothing to find ******\n\n";
                    break;
                }
                string search, replace;
                cout << "Enter text to find, (<-Back to return to main menu): ";
                cin.ignore();
                getline(cin, search, '\n');
                if (search != "<-Back" && search != "<-back") {
                    cout << "Enter text to replace: ";
                    getline(cin, replace, '\n');
                    if (findR(search, replace)) {
                        cout << "\n*** Text found & replaced successfully ***\n";
                    } else {
                        cout << "\n****** Text not found ******\n\n";
                    }
                }
                break;
            }
            case 5:
                undo();
                break;
            case 6:
                redo();
                break;
            case 7: {
                if (text.empty()) {
                    cout << "\n****** Nothing to cut ******\n\n";
                    break;
                }
                int start, length;
                while (true) {
                    cout << "Enter start position and length to cut, (-1 to return to main menu): ";

                    if (!(cin >> start) || start < -1) {
                        cout << "\n****** Invalid input for start position. Please enter a valid position ******\n" << endl;
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                        continue;
                    }
                    if (start == -1)
                        break;
                    cin.ignore();

                    cout << "Enter length to cut: ";
                    if (!(cin >> length) || length < 0) {
                        cout << "\n ****** Invalid input for length. Please enter a valid length ******\n" << endl;
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                        continue;
                    }
                    break;
                }
                if (start != -1)
                    cut(start, length);
                break;
            }

            case 8: {
                if (text.empty()) {
                    cout << "\n****** Nothing to copy ******\n" << endl;
                    break;
                }
                int start, length;
                while (true) {
                    cout << "Enter start position and length to copy, (-1 to return to main menu): ";
                    if (!(cin >> start) || start < -1) {
                        cout << "\n****** Invalid input for start position. Please enter a valid position ******\n" << endl;
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                        continue;
                    }
                    if (start == -1)
                        break;
                    cin.ignore();

                    cout << "Enter length to copy: ";
                    if (!(cin >> length) || length < 0) {
                        cout << "\n****** Invalid input for length. Please enter a valid length ******\n" << endl;
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                        continue;
                    }
                    break;
                }
                if(start != -1)
                    copy(start, length);
                break;
            }
            case 9: {
                if (clipboard.empty()) {
                    cout << "\n****** Nothing to paste ******\n" << endl;
                    break;
                }
                int position;
                while (true) {
                    cout << "Enter position to paste, (-1 to return to main menu): ";
                    if (!(cin >> position) || position < -1) {
                        cout << "\n****** Invalid input for paste position. Please enter a valid position ******\n" << endl;
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                        continue;
                    }
                    break;
                }
                if(position != -1)
                    paste(position);
                break;
            }
            case 10:
                viewClipboard();
                break;
            case 11:
                displayContent();
                break;
            case 12: {
                string filename;
                cout << "Enter the filename to save to, (<-Back to return to main menu): ";
                cin >> filename;
                if(filename != "<-Back" && filename != "<-back")
                    saveToFile(filename);
                break;
            }
            case 13: {
                string filename;
                cout << "Enter the filename to load from, (<-Back to return to main menu): ";
                cin >> filename;
                if(filename != "<-Back" && filename != "<-back")
                    loadFromFile(filename);
                break;
            }
            case 14:{
                cout << "==========Exiting the text editor==========" << endl;
                exit(1);
            }
            default:{
                cout << "\n****** Invalid choice. Please enter a number from 1 to 13 ******\n\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
        }
    }
}
