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

void TextEditor::insert(const string& str) {
    saveState();
    text += str;
    currentText = text;
}

void TextEditor::deleteSubstring(int start, int length) {
    saveState();
    text.erase(start, length);
    currentText = text;
}

bool TextEditor::find(const string& search) {
    myTree.rebuild(text);
    return myTree.search(search);
}

void TextEditor::undo() {
    if(!initialState){
        initialState= true;
        text="";
        return;
    }
    if (previousText.empty()) {
        cout << "Nothing to undo." << endl;
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

void TextEditor::copy(int start, int length) {
    clipboard = text.substr(start, length);
}

void TextEditor::paste(int position) {
    saveState();
    text.insert(position, clipboard);
    currentText = text;
}
void TextEditor::cut(int start, int length) {
    copy(start, length);
    deleteSubstring(start, length);
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
        cout << "4. Undo\n";
        cout << "5. Redo\n";
        cout << "6. Cut\n";
        cout << "7. Copy Text\n";
        cout << "8. Paste Text\n";
        cout << "9. Display Content\n";
        cout << "10. Save to File\n";
        cout << "11. Load from File\n";
        cout << "12. Exit\n";

        cout << "Enter your choice: ";

        string content;

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter text to insert, <-Back to return to main menu: ";
                cin.ignore();  // Consume the newline character.
                getline(cin, content);
                if(content != "<-Back" && content != "<-back")
                    insert(content);
                break;
            }
            case 2: {
                int start, length;
                cout << "Enter start position and length to delete, -1 to return to main menu: ";
                cin >> start >> length;
                if(start != -1)
                    deleteSubstring(start, length);
                break;
            }
            case 3: {
                string search;
                cout << "Enter text to find, <-Back to return to main menu: ";
                cin.ignore();
                getline(cin, search);
                if(search != "<-Back" && search != "<-back") {
                    bool found = find(search);
                    if (found) {
                        cout << "Text found. " << endl;
                    } else {
                        cout << "Text not found." << endl;
                    }
                }
                break;
            }
            case 4:
                undo();
                break;
            case 5:
                redo();
                break;
            case 6: {
                int start, length;
                cout << "Enter start position and length to cut, -1 to return to main menu: ";
                cin >> start >> length;
                if(start != -1)
                    cut(start, length);
                break;
            }
            case 7: {
                int start, length;
                cout << "Enter start position and length to copy, -1 to return to main menu: ";
                cin >> start >> length;
                if(start != -1)
                    copy(start, length);
                break;
            }
            case 8: {
                int position;
                cout << "Enter position to paste, <-Back to return to main menu: ";
                cin >> position;
                if(position != -1)
                    paste(position);
                break;
            }
            case 9:
                displayContent();
                break;
            case 10: {
                string filename;
                cout << "Enter the filename to save to, <-Back to return to main menu: ";
                cin >> filename;
                if(filename != "<-Back" && filename != "<-back")
                    saveToFile(filename);
                break;
            }
            case 11: {
                string filename;
                cout << "Enter the filename to load from, <-Back to return to main menu: ";
                cin >> filename;
                if(filename != "<-Back" && filename != "<-back")
                    loadFromFile(filename);
                break;
            }
            case 12:
                cout << "Exiting the text editor." << endl;
                exit(1);

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}