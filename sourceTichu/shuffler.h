#include<iostream>

using namespace std;

/**
 * Shuffles an array using an algorithm that initially iterates an index i from the last to second element.
 * And then a second index j is used to select an element randomly between the first element and the i-th element.
 * And finally The elements in positions i and j are swapped.
 *
 * @param myArray the array to be shuffled.
 * @param myArraySize the index of the array to be shuffled.
 */
template <class X> void shuffle(X** myArray, int myArraySize){

    for(int i = myArraySize - 1; i >= 1 ; i--){
        int j = rand() % (i+1);
        X* tmp = myArray[i];
        myArray[i] = myArray[j];
        myArray[j] = tmp;
    }
}

/**
 * "Cuts" an array at a random point similarly to a deck of cards (i.e. getting the last part of
 * the cards and moving it before the first part of the cards).
 *
 * @param myArray the array to be "cut".
 * @param myArraySize the index of the array to be "cut".
 */
template <class X> void cut(X** myArray, int myArraySize){

    int cutPoint = rand() % myArraySize;
    //cutPoint must be positive. There is no point in allocating 0 memory
    if(cutPoint > 0){
        X** tempArray = new X*[cutPoint];
        //We copy the first cutPoint elements of our matrix to the tempArray
        for(int i = 0; i < cutPoint; i++){
            tempArray[i] = myArray[i];
        }
        /*We move the rest (myArraySize - cutPoint) elements
        of myArray to the beginning of the array*/
        for(int i = cutPoint; i < myArraySize; i++){
            myArray[i - cutPoint] = myArray[i];
        }
        /* Now what's left is to concatenate the tempArray with myArray
        having the tmp array starting from the element of myArray
        with index myArraySize - cutPoint */
        //This loop is iterated for cutPoint elements
        for(int i = myArraySize - cutPoint; i < myArraySize; i++){
            myArray[i] = tempArray[i - myArraySize + cutPoint];
        }
        //We must delete tempArray to free memory in heap
        delete[] tempArray;
    }
}
