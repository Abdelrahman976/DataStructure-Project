#include <string>
using namespace std;

#ifndef SUFFIXTREE
#define SUFFIXTREE
class SuffixTree {
    public:
        SuffixTree();
        SuffixTree(string);
        SuffixTree(SuffixTree&);
        ~SuffixTree();
        void construct(string);
        bool empty() const;
        bool search(const string) const;
        SuffixTree operator=(SuffixTree&);
        void display(ostream&);
        void rebuild(string item);
    private:
        void quicksort(string arr[], int low, int high);
        string* myArray;
        int myCapacity;
};

ostream& operator<<(ostream& out, SuffixTree& s);
#endif
