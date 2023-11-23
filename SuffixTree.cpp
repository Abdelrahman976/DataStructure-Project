#include <iostream>
#include <string>
#include "SuffixTree.h"
using namespace std;

SuffixTree::SuffixTree(string str):myCapacity(str.length()){
    myArray = new string[myCapacity];
    construct(str );
    myCapacity = str.length() ;
}

SuffixTree::SuffixTree(SuffixTree & other):myCapacity(other.myCapacity) {
    myArray=new string [myCapacity];
    for (int i = 0; i < myCapacity; ++i) {
        myArray[i]=other.myArray[i];
    }
}

SuffixTree::~SuffixTree(){
    delete[] myArray;
}

void SuffixTree::construct(string item){
    for (int i=0; i<item.length(); i++) {
        for (int j = i; j < item.length(); ++j) {
            myArray[i] += item[j];
        }
    }
    quicksort(myArray, 0, item.length()-1);
}

void swap(string &a, string &b) {
    string temp = a;
    a = b;
    b = temp;
}

int partition(string arr[], int low, int high) {
    string pivot = arr[low];
    int i = low + 1;
    int j = high;
    while (true) {
        while (i <= j && arr[i] <= pivot) {
            i++;
        }
        while (i <= j && arr[j] > pivot) {
            j--;
        }
        if (i > j) {
            break;
        }
        swap(arr[i], arr[j]);
    }
    swap(arr[low], arr[j]);
    return j;
}

void SuffixTree::quicksort(string arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quicksort(arr, low, pivotIndex - 1);
        quicksort(arr, pivotIndex + 1, high);
    }
}


void SuffixTree::display(ostream & out) {
    for (int k = 0; k < myCapacity; ++k) {
        out << myArray[k];
        if (k != myCapacity-1) out << ", ";
    }
    out << endl;
}

bool SuffixTree::search(const string suffix) const {
    bool found=false;
    for (int i = 0; i < myCapacity; ++i) {
        if (found) {
            return found;
        }
        int j = 0;
        for (char x: suffix) {
            if (tolower(myArray[i][j]) == tolower(x)) {
                found = true;
            } else {
                found = false;
                break;
            }
            j++;
        }
    }
    return found;
//    for (int i = 0; i < myCapacity; ++i) {
//        if (myArray[i].find(suffix) != string::npos){
//            return true;
//        }
//    }
//    return false;
}



bool SuffixTree::empty() const {
    return myCapacity==0;
}

SuffixTree SuffixTree::operator=(SuffixTree & other) {
    if(this != &other){
        if(myCapacity!=other.myCapacity){
            SuffixTree::~SuffixTree();
            myCapacity=other.myCapacity;
            myArray=new string [myCapacity];
        }
        for (int i = 0; i < myCapacity; ++i) {
            myArray[i]=other.myArray[i];
        }
    }
    return *this;
}

ostream& operator<<(ostream& out, SuffixTree& s){
    s.display(out);
    return out;
}
void SuffixTree::rebuild(string str){
    this->~SuffixTree();
    myArray = new string[str.length()];
    construct(str);
    myCapacity = str.length();
}

