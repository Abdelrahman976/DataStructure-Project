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
        return;
    }
    text = previousText;
}

void TextEditor::redo() {
    if (currentText == text) {
        cout << "Nothing to redo." << endl;
        return;
    }
    text = currentText;
}

void TextEditor::viewClipboard() {
    cout << "\n\n*** Clipboard: " << clipboard << " ***\n\n";
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
        cout << "Error: Unable to save the text to " << filename << endl;
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
        cout << "Error: Unable to load text from " << filename << endl;
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
        cout << "7. Cut\n";
        cout << "8. Copy Text\n";
        cout << "9. Paste Text\n";
        cout << "10. Display Content\n";
        cout << "11. Save to File\n";
        cout << "12. Load from File\n";
        cout << "13. Exit\n";

        cout << "Enter your choice: ";

        string content;

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter text to insert, (<-Back to return to main menu): ";
                cin.ignore();  // Consume the newline character.
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
                        cerr << "\n\n****** Invalid position ******\n\n";
                }
                break;
            }
            case 2: {
                string text;
                cout << "Enter text to delete, (<-Back to return to main menu): ";
                cin.ignore(); // Consume the newline character.
                getline(cin, text, '\n');
                if(text != "<-Back" && text != "<-back") {
                    if (findR(text, "")){
                        cout << "\n\n*** Text has been deleted successfully ***\n\n";
                    }else{
                        cerr << "\n\n****** Text not found ******\n\n";
                    }
                }
                break;
            }
            case 3: {
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
                        cout << "Text not found." << endl;
                    }
                }
                break;
            }
            case 4: {
                string search, replace;
                cout << "Enter text to find, (<-Back to return to main menu): ";
                cin.ignore();
                getline(cin, search, '\n');
                if (search != "<-Back" && search != "<-back") {
                    cout << "Enter text to replace: ";
                    getline(cin, replace, '\n');
                    if (findR(search, replace)) {
                        cout << "\n\n*** Text found & replaced successfully ***\n\n";
                    } else {
                        cout << "Text not found.\n";
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
                int start, length;
                cout << "Enter start position and length to cut, (-1 to return to main menu): ";
                cin >> start >> length;
                if(start != -1)
                    cut(start, length);
                break;
            }
            case 8: {
                int start, length;
                cout << "Enter start position and length to copy, (-1 to return to main menu): ";
                cin >> start >> length;
                if(start != -1)
                    copy(start, length);
                break;
            }
            case 9: {
                int position;
                cout << "Enter position to paste, (-1 to return to main menu): ";
                cin >> position;
                if(position != -1)
                    paste(position);
                break;
            }
            case 10:
                displayContent();
                break;
            case 11: {
                string filename;
                cout << "Enter the filename to save to, (<-Back to return to main menu): ";
                cin >> filename;
                if(filename != "<-Back" && filename != "<-back")
                    saveToFile(filename);
                break;
            }
            case 12: {
                string filename;
                cout << "Enter the filename to load from, (<-Back to return to main menu): ";
                cin >> filename;
                if(filename != "<-Back" && filename != "<-back")
                    loadFromFile(filename);
                break;
            }
            case 13:
                cout << "Exiting the text editor." << endl;
                exit(1);

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}
