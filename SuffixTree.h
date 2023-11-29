#include <string>
using namespace std;

#ifndef SUFFIXTREE
#define SUFFIXTREE
class SuffixTree {
    public:
        SuffixTree(string s="");
        SuffixTree(SuffixTree&);
        ~SuffixTree();
        void rebuild(string); // Re-Construct the tree with a given string
        void insert(const string); // insert prefix to original text
        void erase(const string); // Erase a prefix from the original text
        bool empty() const; // Check for empty tree
        bool search(const string, int&) const; // Search for suffixes using a given prefix
        SuffixTree operator=(SuffixTree&);
        void display(ostream&);

    private:
        void quicksort(string arr[], int indexesArr[], int low, int high);
        string* myArray; // Array of Suffixes
        int* Indexes; // Array of Each Suffix Index
        string text; // Original Text
        int myCapacity; // Capacity of Tree
};

ostream& operator<<(ostream& out, SuffixTree& s);
#endif
