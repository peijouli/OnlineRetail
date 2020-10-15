#include <vector> 
#include <iostream>
#include <algorithm>
#include <math.h> 

// spliting into k pieces instead of 2

/*merge sort:
MergeSort(arr[], l,  r)
If r > l
     1. Find the middle point to divide the array into two halves:  
             middle m = (l+r)/2
     2. Call mergeSort for first half:   
             Call mergeSort(arr, l, m)
     3. Call mergeSort for second half:
             Call mergeSort(arr, m+1, r)
     4. Merge the two halves sorted in step 2 and 3:
             Call merge(arr, l, m, r)
*/

template <class T, class Comparator>
void merge(std::vector<T>& myArray, int k, int l, int r, Comparator comp, int bounds[]){
    int range = floor( (r-l+1) / k);
    int indice[k];
    for (int i =0; i < k; i++){
        indice[i] = 0;
    }
    std::vector<T> temp;
    for (int i = 0; i <  r - l + 1; i ++){
        T min = myArray[i]; 
        T* min_value = &min;
        int partition = -1; 
        for (int j = 0; j < k ; j++){  
            if ( ( (min_value == NULL) || ( (l+j*range + indice[j] ) <= bounds[j]) ) 
                && (  comp(myArray[l+ j*range + indice[j]], min ) ) ) {
                min = myArray[l + j*(range) + indice[j] ];
                partition = j; 
            }
        }
        temp.push_back(min);
        indice[partition]++;
    }
}

template <class T, class Comparator>
void mergeDivide (std::vector<T>& myArray, int l, int r, int k, Comparator comp) { //check if need to change to mergeort 
    int m = floor ( (r-l+1)/k);
    int min_idx =0;
    if ((r-l+1 < k) || (r >= (int)myArray.size()))
    {
        for (int i = 0; i < (int)myArray.size()-1 ; i++) 
        {
            min_idx = i;
            for (int j = i+1 ; j < (int)myArray.size() ; j++) 
            {
                if (comp(myArray[j], myArray[min_idx]))
                    min_idx = j;
            }
            std::swap(myArray[i], myArray[min_idx]);
        } 
    }

    else {
        int last = 0;
        // int bounds[myArray.size()];
        int bounds[k];
        for (int i = 0; i < k ; i++){
            mergeDivide(myArray, last, last + m + ( ((r-l+1)%k > i) ?0:-1) , k, comp);
            last += m + ((r-l+1)%k > i ?1:0);
            bounds[k] = last - 1;
        }
        merge(myArray, 2, l, r, comp, bounds);
    }

    
}


template <class T, class Comparator>
void mergeSort (std::vector<T>& myArray, Comparator comp){
    mergeDivide (myArray, 0, myArray.size()-1, 2, comp);
}
