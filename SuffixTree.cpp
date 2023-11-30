#include <iostream>
#include <string>
#include "SuffixTree.h"
using namespace std;

SuffixTree::SuffixTree(string item): myCapacity(0){
    rebuild(item);
}

void SuffixTree::rebuild(string item) {
    if (!empty()) // To Check if it is Called by constructor or by another function
        this->~SuffixTree();
    myCapacity = item.length();
    text = item;
    myArray = new string[myCapacity];
    Indexes = new int[myCapacity];
    for (int i=0; i<item.length(); i++) {
        for (int j = i; j < item.length(); ++j) {
            myArray[i] += item[j];
        }
        Indexes[i] = i;
    }
    quicksort(myArray, Indexes, 0, item.length()-1);
}

SuffixTree::SuffixTree(SuffixTree & other):myCapacity(other.myCapacity) {
    myArray=new string [myCapacity];
    Indexes  =new int [myCapacity];
    for (int i = 0; i < myCapacity; ++i) {
        myArray[i]=other.myArray[i];
        Indexes[i]=other.Indexes[i];
    }
}

SuffixTree::~SuffixTree(){
    delete[] myArray;
    delete[] Indexes;
}
template<typename T>
void Swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

int partition(string arr[], int indexesArr[], int low, int high) {
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
        Swap<string>(arr[i], arr[j]);
        Swap<int>(indexesArr[i], indexesArr[j]);
    }
    Swap<string>(arr[low], arr[j]);
    Swap<int>(indexesArr[low], indexesArr[j]);
    return j;
}

void SuffixTree::quicksort(string arr[], int indexesArr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, indexesArr, low, high);
        quicksort(arr, indexesArr, low, pivotIndex - 1);
        quicksort(arr, indexesArr, pivotIndex + 1, high);
    }
}


void SuffixTree::display(ostream & out) {
    for (int k = 0; k < myCapacity; ++k) {
        out << "Element: " << myArray[k];
        out << ", Index: " << Indexes[k] << endl;
    }
    out << endl;
}

bool SuffixTree::search(const string prefix, int& ind) const {
    bool found=false;
    for (int i = 0; i < myCapacity; ++i) {
        if (found) {
            ind = Indexes[i-1];
            return found;
        }
        int j = 0;
        for (char x: prefix) {
            if (tolower(myArray[i][j]) == tolower(x)) {
                found = true;
            } else {
                ind = -1;
                found = false;
                break;
            }
            j++;
        }
    }
    return found;
}

void SuffixTree::insert(const string str) {
    if (str.empty()) return;
    rebuild(text + str);
}

void SuffixTree::erase(const string str) {
    int ind;
    if (str.empty() || !search(str, ind)) return;
    string temp = "";
    for (int i = 0; i < myCapacity; ++i) {
        if (i < ind || i >= ind + str.length()) {
            temp += text[i];
        }
    }
    rebuild(temp);
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
            Indexes = new int[myCapacity];
        }
        for (int i = 0; i < myCapacity; ++i) {
            myArray[i]=other.myArray[i];
            Indexes[i]=other.Indexes[i];
        }
    }
    return *this;
}

ostream& operator<<(ostream& out, SuffixTree& s){
    s.display(out);
    return out;
}