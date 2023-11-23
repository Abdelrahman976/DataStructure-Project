#include <iostream>
#include <string>
#include "SuffixTree.h"
using namespace std;

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

SuffixTree::SuffixTree(string str){
    myArray = new string[str.length()+1];
    construct(str + '$');
    mySize = str.length() + 1;
}

SuffixTree::~SuffixTree(){
    delete[] myArray;
}

void SuffixTree::quicksort(string arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quicksort(arr, low, pivotIndex - 1);
        quicksort(arr, pivotIndex + 1, high);
    }
}

void SuffixTree::construct(string item){
    for (int i=0; i<item.length(); i++) {
        for (int j = i; j < item.length(); ++j) {
            myArray[i] += item[j];
        }
    }
    quicksort(myArray, 0, item.length()-1);
}

void SuffixTree::display(ostream & out) {
    for (int k = 0; k < mySize; ++k) {
        cout << myArray[k];
        if (k != mySize-1) out << ", ";
    }
    cout << endl;
}
