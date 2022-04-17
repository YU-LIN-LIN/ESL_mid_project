#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
using namespace std;

void swap(int &p1, int &p2){
    int temp;
    temp = p1;
    p1 = p2;
    p2 = temp;
}

int main(void) {

    int i, j;
    int *array;
    array = new int [9];
    int max_addr;

    ifstream input;
    input.open("data.txt");
    for (i = 0; i < 9; i++) {
        input >> array[i];
    }
    input.close();

    cout << "original sequence : ";
    for (i = 0; i < 9; i++) {
        cout << array[i] << " ";
    } cout << endl;

    for (i = 8; i > 0; i--) {
        for (j = ((i + 1) / 2 - 1); j >= 0; j--) {
            if (j == 0) {
                if (i >= 1 && array[1] > array[2] && array[1] > array[0]) {
                    swap(array[1], array[0]);
                } else if (i >= 2 && array[2] > array[1] && array[2] > array[0]) {
                    swap(array[2], array[0]);
                }
            } else {
                if (i >= (2*j+1) && array[2*j+1] > array[2*j+2] && array[2*j+1] > array[j]) {
                    swap(array[2*j+1], array[j]);
                } else if (i >= (2*j+2) && array[2*j+2] > array[2*j+1] && array[2*j+2] > array[j]) {
                    swap(array[2*j+2], array[j]);
                }
            }
        }
        swap(array[0], array[i]);
    }

    ofstream output;
    output.open("output.txt");

    if (!output.is_open()) {
        cout << "Failed to open file." << endl;
    } else {
        for (i = 1; i <= 9; i++) {
        output << array[i] << " ";
        }
        output.close();
    }

    cout << "final sequence : ";
    for (i = 0; i < 9; i++) {
        cout << array[i] << " ";
    } cout << endl;


    delete [] array;

    return 0;
}
