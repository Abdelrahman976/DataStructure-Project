#include "SuffixTree.h"
#include "TextEditor.h"
using namespace std;

int main() {
    int x;
    SuffixTree s("mississippi")
    ;
    s.erase("ppi");
    cout << s << endl;
    s.insert("ppi");
    cout << s << endl;
    if (s.search("issi", x))
        cout << "Text Found at index: " << x << endl;
    else
        cout << "Text Not Found\n";
}