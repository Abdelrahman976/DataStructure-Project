#include <string>
using namespace std;

#ifndef SUFFIXTREE
#define SUFFIXTREE
class SuffixTree {
 public:
  SuffixTree(string);
  SuffixTree(SuffixTree&);
  ~SuffixTree();
  void construct(string);
  bool empty() const;
  bool search(const string) const;
  SuffixTree operator=(SuffixTree&);
  void display(ostream&);
  friend ostream& operator<<(ostream& out, SuffixTree& s);
private:
    void quicksort(string arr[], int low, int high);
    string* myArray;
    int mySize;
};

ostream& operator<<(ostream& out, SuffixTree& s){
    s.display(out);
    return out;
}
#endif
