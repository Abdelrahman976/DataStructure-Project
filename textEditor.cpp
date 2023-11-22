#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class SuffixTree {
public:
    SuffixTree() {
        root = new Node();
        text = "";
        currentText = "";
    }

    void insert(const string& str) {
        saveState();
        text += str;
        currentText = text;
        buildSuffixTree();
    }

    void deleteSubstring(int start, int length) {
        saveState();
        text.erase(start, length);
        currentText = text;
        buildSuffixTree();
    }

    int find(const string& search) {
        int n = text.length();
        int m = search.length();

        for (int i = 0; i <= n - m; i++) {
            int j = 0;
            while (j < m && search[j] == text[i + j]) {
                j++;
            }
            if (j == m) {
                return i;
            }
        }
        return -1;
    }

    void undo() {
        if (previousText.empty()) {
            cout << "Nothing to undo." << endl;
            return;
        }
        text = previousText;
//        currentText = text;
    }

    void redo() {
        if (currentText == text) {
            cout << "Nothing to redo." << endl;
            return;
        }
        text = currentText;
    }

    void copy(int start, int length) {
        clipboard = text.substr(start, length);
    }

    void paste(int position) {
        saveState();
        text.insert(position, clipboard);
        currentText = text;
        buildSuffixTree();
    }
    void cut(int start, int length) {
        copy(start, length);
        deleteSubstring(start, length);
    }
    void displayContent() {
        cout << "Text Editor Content:\n";
        cout << "=================================================================================================\n";
        cout << text << endl;
        cout << "=================================================================================================\n";
    }

    void saveToFile(const string& filename) {
        ofstream outFile(filename); // Fix: Include the header for ofstream
        if (outFile.is_open()) {
            outFile << text;
            outFile.close();
            cout << "Text saved to " << filename << endl;
        } else {
            cout << "Error: Unable to save the text to " << filename << endl;
        }
    }

    void loadFromFile(const string& filename) {
        // ...
        ifstream inFile(filename); // Fix: Include the header for ifstream
        if (inFile.is_open()) {
            getline(inFile, text, '\0');
            currentText = text;
            inFile.close();
            cout << "Text loaded from " << filename << endl;
        } else {
            cout << "Error: Unable to load text from " << filename << endl;
        }
    }

private:
    class Node {
    public:
        Node() {
            for (int i = 0; i < 256; i++) {
                children[i] = nullptr;
            }
            start = -1;
            end = -1;
        }

        Node* children[256];
        int start;
        int end;
    };

    Node* root;
    string text;
    string currentText;
    string clipboard;
    string previousText;

    void saveState() {
        previousText = text;
    }

    void buildSuffixTree() {
        root = new Node();
        int n = text.length();
        for (int i = 0; i < n; i++) {
            insertSuffix(text.substr(i));
        }
    }

    void insertSuffix(const string& suffix) {
        Node* current = root;
        int length = suffix.length();
        int i = 0;

        while (i < length) {
            char c = suffix[i];
            if (!current->children[c]) {
                current->children[c] = new Node();
                current->children[c]->start = i + current->start;
                current->children[c]->end = text.length() - 1;
                return;
            }

            int j = current->children[c]->start;
            while (j <= current->children[c]->end && i < length && text[j] == suffix[i]) {
                j++;
                i++;
            }

            if (j > current->children[c]->end) {
                current = current->children[c];
            } else {
                Node* newChild = new Node();
                newChild->start = current->children[c]->start;
                newChild->end = j - 1;
                current->children[c]->start = j;
                current->children[c]->end = text.length() - 1;
                newChild->children[suffix[i]] = new Node();
                newChild->children[suffix[i]]->start = i + current->start;
                newChild->children[suffix[i]]->end = text.length() - 1;
                current->children[c]->children[text[j]] = newChild;
                return;
            }
        }
    }
};

void menu(SuffixTree editor){
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
                    editor.insert(content);
                break;
            }
            case 2: {
                int start, length;
                cout << "Enter start position and length to delete, -1 to return to main menu: ";
                cin >> start >> length;
                if(start != -1)
                    editor.deleteSubstring(start, length);
                break;
            }
            case 3: {
                string search;
                cout << "Enter text to find, <-Back to return to main menu: ";
                cin.ignore();
                getline(cin, search);
                if(search != "<-Back" && search != "<-back") {
                    int found = editor.find(search);
                    if (found != -1) {
                        cout << "Found at position: " << found << endl;
                    } else {
                        cout << "Text not found." << endl;
                    }
                }
                break;
            }
            case 4:
                editor.undo();
                break;
            case 5:
                editor.redo();
                break;
            case 6: {
                int start, length;
                cout << "Enter start position and length to cut, -1 to return to main menu: ";
                cin >> start >> length;
                if(start != -1)
                    editor.cut(start, length);
                break;
            }
            case 7: {
                int start, length;
                cout << "Enter start position and length to copy, -1 to return to main menu: ";
                cin >> start >> length;
                if(start != -1)
                    editor.copy(start, length);
                break;
            }
            case 8: {
                int position;
                cout << "Enter position to paste, <-Back to return to main menu: ";
                cin >> position;
                if(position != -1)
                    editor.paste(position);
                break;
            }
            case 9:
                editor.displayContent();
                break;
            case 10: {
                string filename;
                cout << "Enter the filename to save to, <-Back to return to main menu: ";
                cin >> filename;
                if(filename != "<-Back" && filename != "<-back")
                    editor.saveToFile(filename);
                break;
            }
            case 11: {
                string filename;
                cout << "Enter the filename to load from, <-Back to return to main menu: ";
                cin >> filename;
                if(filename != "<-Back" && filename != "<-back")
                    editor.loadFromFile(filename);
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
int main() {
    SuffixTree editor;

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
                    editor.insert(content);
                break;
            }
            case 2: {
                int start, length;
                cout << "Enter start position and length to delete, -1 to return to main menu: ";
                cin >> start;
                if(start != -1){
                    cin >> length;
                    editor.deleteSubstring(start, length);
                }
                break;
            }
            case 3: {
                string search;
                cout << "Enter text to find, <-Back to return to main menu: ";
                cin.ignore();
                getline(cin, search);
                if(search != "<-Back" && search != "<-back") {
                    int found = editor.find(search);
                    if (found != -1) {
                        cout << "Found at position: " << found << endl;
                    } else {
                        cout << "Text not found." << endl;
                    }
                }
                break;
            }
            case 4:
                editor.undo();
                break;
            case 5:
                editor.redo();
                break;
            case 6: {
                int start, length;
                cout << "Enter start position and length to cut, -1 to return to main menu: ";
                cin >> start;
                if(start != -1){
                    cin >> length;
                    editor.cut(start, length);
                }
                break;
            }
            case 7: {
                int start, length;
                cout << "Enter start position and length to copy, -1 to return to main menu: ";
                cin >> start;
                if(start != -1){
                    cin >> length;
                    editor.copy(start, length);
                }
                break;
            }
            case 8: {
                int position;
                cout << "Enter position to paste, <-Back to return to main menu: ";
                cin >> position;
                if(position != -1)
                    editor.paste(position);
                break;
            }
            case 9:
                editor.displayContent();
                break;
            case 10: {
                string filename;
                cout << "Enter the filename to save to, <-Back to return to main menu: ";
                cin >> filename;
                if(filename != "<-Back" && filename != "<-back")
                    editor.saveToFile(filename);
                break;
            }
            case 11: {
                string filename;
                cout << "Enter the filename to load from, <-Back to return to main menu: ";
                cin >> filename;
                if(filename != "<-Back" && filename != "<-back")
                    editor.loadFromFile(filename);
                break;
            }
            case 12:
                cout << "Exiting the text editor." << endl;
                exit(1);

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}