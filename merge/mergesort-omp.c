#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

/* Function to merge */
void merge(int arr[], int l, int m, int r);

// Utility function to find minimum of two integers
int min(int x, int y) { return (x<y)? x :y; }

/* Iterative mergesort function to sort arr[0...n-1] */
void mergeSort(int arr[], int n)
{
    int curr_size;  // For current size of subarrays to be merged
                // curr_size varies from 1 to n/2
    int left_start; // For picking starting index of left subarray
                // to be merged
    for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size)
    {
        // Start point different subarrays of current size
        for (left_start=0; left_start<n-1; left_start += 2*curr_size)
        {
            // Find ending point of left subarray. mid+1 is starting
            // point of right
            int mid = left_start + curr_size - 1;

            int right_end = min(left_start + 2*curr_size - 1, n-1);

            // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
            if (mid < right_end) merge(arr, left_start, mid, right_end);
        }
    }
}

/* Function to merge the two haves arr[l..m] and arr[m+1..r] of array arr[]
*/
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int *L, *R;
    L = (int *)malloc(sizeof(int) * n1);
    R = (int *)malloc(sizeof(int) * n2);
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there are any */

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
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
