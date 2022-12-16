#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

// Swaps two elements.
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* Partitions the array from the low to high index inclusive such that elements to the left are less than the pivot and elements to the right are greater than the pivot.*/
int partition(int * arr, int low, int high)
{
    int pivot = arr[high]; // Chooses the highest-index element as the pivot.
    int i = (low - 1); // Index of the current new pivot position.
  
    // Swap all elements less than the pivot to be to the left of the new pivot index:
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            // If element is less than the pivot then swap it to left side:
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/* Divide and conquers the array by sorting both sides of a chosen pivot in parallel.*/
void quicksort(int * a, int p, int r)
{
    if (p < r){ 
        int div = partition(a, p, r); 

// Parallel sections
#pragma omp parallel sections
        {
#pragma omp section
            {
                // sort the first half.
                quicksort(a, p, div - 1);
            }
#pragma omp section
            {
                // sort the second half.
                quicksort(a, div + 1, r);
            }
        }
    }
}



// Routes to specific number sort implementation.
void sort(int arr[], int n){
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    quicksort(arr, 0, n);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    float delta_s = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0f;
    printf("time: %fs\n", delta_s);
}



// Reads file of comma-separated numbers (ex: "1,2,3,4,5") into output int array and count.
void nums_from_file(char* numsFilename, int** outNums, int* outCount)
{
    FILE* file = fopen(numsFilename, "r");
    int numsCount = 0;
    int _;
    for (; fscanf(file, "%d,", &_) == 1; numsCount++);

    // Put file pointer back at start to scan in all numbers:
    rewind(file);
    
    // Define unsorted numbers array:
    int* nums = malloc(numsCount * sizeof(int));

    for (int i = 0; i < numsCount; i++)
    {
        // Scan number into array:
        fscanf(file, "%d,", &nums[i]);
    }

    fclose(file);
    *outNums = nums;
    *outCount = numsCount;
}

// Prints array to console.
void printArray(int *a, int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", a[i]);
    printf("\n");
}

// Writes comma-separated nums to file.
void writeNumsToFile(int* nums, int numsSize, char* numsFilename)
{
    FILE* file = fopen(numsFilename, "w");

    for (int i = 0; i < numsSize - 1; i++){
        fprintf(file, "%d,", nums[i]);
    }
    fprintf(file, "%d", nums[numsSize - 1]); // Leaves comma out of final number.

    fclose(file);
}

// Returns if the given array of size is sorted or not.
int isSorted(int *a, int size)
{
    for(int i = 0; i < size - 1; i++)
        if(a[i] > a[i + 1])
        return 0;
    return 1;
}

// Reads from input file, sorts, then outputs sorted nums to output file.
// (1st arg is input file name, 2nd is output filename, 3rd is 1 if debug mode/printing).
int main(int argc, char *argv[])
{
    char* unsortedNumsFilename = (argc > 1) ? argv[1] : "unsorted-nums.txt";
    char* outputNumsFilename = (argc > 2) ? argv[2] : "output-sorted-nums.txt";
    int debug = (argc > 3) ? atoi(argv[3]) : 0;

    // Load input numbers:
    int *numsToSort = NULL;
    int numsToSortLength;
    nums_from_file(unsortedNumsFilename, &numsToSort, &numsToSortLength);

    // Ensure that input numbers were loaded:
    if (!numsToSort)
    { 
        if (numsToSort) free(numsToSort);
        return (EXIT_FAILURE);
    }

    if (debug == 1){
        // Print input numbers:
        printf("Unsorted Nums (%s):\n", unsortedNumsFilename);
        printArray(numsToSort, numsToSortLength);
        printf("\n");
    }
    printf("Sorting %d numbers...", numsToSortLength);

    // Perform the actual sort:
    sort(numsToSort, numsToSortLength);

    // Save to output file:
    writeNumsToFile(numsToSort, numsToSortLength, outputNumsFilename);
    if (debug == 1){
        // Assert that the sort worked:
        assert(1 == isSorted(numsToSort, numsToSortLength));

        // Print output:
        printf("Sorted Nums (%s):\n", outputNumsFilename);
        printArray(numsToSort, numsToSortLength);
    }

    free(numsToSort);
    return (EXIT_SUCCESS);
}
