#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "sortArray.h"


int main(int argc, char *argv[]) {
    char* fname = argv[1];
    if(access( fname, F_OK ) == -1) {
        printf("error");
        return -1;
    }
    FILE* fp = fopen(fname, "r");

    int len;
    if(fscanf(fp, "%d", &len) != 1)
        return -1;

    int arr[len];
    for(int i = 0; i < len; i++) {
        if(fscanf(fp, "%d", &arr[i]) != 1) {
            return -1;
        }
    }

    fclose(fp);

    sortArray(arr, len);

    return 0;
}

void sortArray(int *arr, int len) {
    int left = 0;
    int right = len-1;
    int temp;

    while (left < right) {
        if(abs(arr[left]) % 2 == 0) { // if left is even
            left++;
        } else if(abs(arr[right]) % 2 == 1) { // if right is odd
            right--;
        } else { // if left is even & right is odd
            temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
        }
    }

    if(arr[left] % 2 == 0) {
        quickSort(arr, 0 , left, 0);
        quickSort(arr, left+1, len-1, 1);
    } else {
        quickSort(arr, 0 , left-1, 0);
        quickSort(arr, left, len-1, 1);
    }

    for (int i = 0; i < len; ++i) {
        printf("%d\t", arr[i]);
    }
}

void quickSort(int arr[], int low, int high, int reverse) {
    if(low < high) {
        int pi = partition(arr, low, high, reverse);

        quickSort(arr, low, pi - 1, reverse);
        quickSort(arr, pi+1, high, reverse);
    }
}

int partition(int arr[], int low, int high, int reverse) {
    int pivot = arr[low];
    int i = low;
    int temp;

    for (int j = low; j <= high; j++) {
        if((!reverse && arr[j] < pivot) || (reverse && arr[j] > pivot)) {
            i++;
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }

        // At end of loop...
        // 1. Unless i = low-1, arr[i] < pivot.
        // 2. If i = j, arr[i] = arr[j] < pivot.
        // 3. If j > i, arr[i+1] > pivot
    }

    temp = arr[i];
    arr[i] = arr[low];
    arr[low] = temp;

    return i;
}