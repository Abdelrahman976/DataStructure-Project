#include <iostream>
#include "SuffixTree.h"
using namespace std;

int main() {
    // Example usage:
    string inputString = "banana";
    SuffixTree suffixTree;
    suffixTree.construct(inputString);

    // Perform a pattern search
    string patternToSearch = "ban";
    suffixTree.searchPattern(patternToSearch, suffixTree);

    return 0;
}