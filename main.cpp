#include <iostream>
#include "SuffixTree.h"
using namespace std;

int main() {
    cout<<"Enter Your Word:";
    string word;
    cin>>word;
    SuffixTree test(word);
    cout<<test<<endl;
    return 0;
}