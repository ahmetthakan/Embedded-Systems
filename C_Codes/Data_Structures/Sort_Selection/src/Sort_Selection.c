/**
============================================================================================
 * @file       Sort_Selection.c
 * @author     Ekrem Demir
 * @date       23-08-2023
 * @brief      Implementation of the Selection Sort algorithm.
 *
 * Description: This program demonstrates the Selection Sort algorithm
 *              on an integer array and prints the sorted result.
 * Notes: By default the order is in ASCENDING. To change it to DESCENDING
 *        change the comparison symbol from "<" to ">"
============================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define type alias for data
typedef uint8_t dataType;

dataType selectionSort(dataType *data, dataType size);
void printArray(dataType *data, dataType size);

/**
 * @brief   Main function to test selection sort.
 * @return  int Return status (typically 0 for success).
 */
int main(void)
{
    dataType testArr[25] = {
        43, 5, 23, 56, 78, 12, 9, 3, 45, 67,
        89, 0, 34, 7, 26, 19, 82, 21, 47, 94,
        11, 68, 72, 33, 50
    };
    dataType arrayLength = sizeof(testArr) / sizeof(testArr[0]);

    printf("Unsorted Array:\n");
    printArray(testArr, arrayLength);

    // Call the selection sort algorithm
    if(selectionSort(testArr, arrayLength))
    {
    	printf("Unable to sort!!!:\n");
    }
    else
    {
        printf("Sorted Array:\n");
        printArray(testArr, arrayLength);
    }


}

/**
 * @brief   Implements the Selection Sort algorithm.
 * @param   data Pointer to data array.
 * @param   size Size of the data array.
 */
dataType selectionSort(dataType *data, dataType size)
{
	if(size <= 0)
	{
		return 1;
	}
    dataType tempValue = 0;
    dataType minIndex;

    for (dataType i = 0; i < size; ++i) {
        minIndex = i; // Assume current index has minimum value
        for (dataType j = i + 1; j < size; ++j) {
            // Find the index with the minimum value

            if (data[j] > data[minIndex]) {
                minIndex = j;
            }
        }
        // Swap the values at i and minIndex
        tempValue = data[i];
        data[i] = data[minIndex];
        data[minIndex] = tempValue;
    }

    return 0;
}

/**
 * @brief   Print elements of an array.
 * @param   data Pointer to data array.
 * @param   size Size of the data array.
 */
void printArray(dataType *data, dataType size)
{
    for (dataType i = 0; i < size; ++i) {
        printf("%d ", data[i]);
    }
    printf("\n");
}
