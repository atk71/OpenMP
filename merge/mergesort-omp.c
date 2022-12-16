#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

/* Function to merge */
void merge (int arr[], int n, int m);

// Utility function to find minimum of two integers
int min(int x, int y) { return (x<y)? x :y; }

/* Recursive mergesort function to sort arr[0...n-1] */
void mergeSort (int *arr, int n) 
{
    if (n < 2)
        return;
    int m = n / 2;

    // Parallel sections
    #pragma omp parallel sections
    {
        // sort the first half.
        #pragma omp section
        {
            mergeSort(arr, m);
        }
        // sort the second half.
        #pragma omp section
        {
            mergeSort(arr + m, n - m);
        }
    }
    // merging the two sections outside parallel region
    merge(arr, n, m);
}

void merge (int *a, int n, int m) 
{
    int i, j, k;
    int *x = malloc(n * sizeof (int));
    for (i = 0, j = m, k = 0; k < n; k++) {
        x[k] = j == n      ? a[i++]
             : i == m      ? a[j++]
             : a[j] < a[i] ? a[j++]
             :               a[i++];
    }
    for (i = 0; i < n; i++) {
        a[i] = x[i];
    }
    free(x);
}

void sort(int arr[], int n){
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  mergeSort(arr, n);

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
    assert(1 == isSorted(numsToSort, numsToSortLength));
    if (debug == 1){
    //if (1) {
        // Assert that the sort worked:

        // Print output:
        printf("Sorted Nums (%s):\n", outputNumsFilename);
        printArray(numsToSort, numsToSortLength);
    }
    free(numsToSort); // Free sorted array now that it has been outputted.

    return (EXIT_SUCCESS);
}
