/**
 * @file       main.c
 * @author     Ekrem Demir
 * @version    1.0
 * @date       23-08-2023
 * @brief      Queue operations demonstration program.
 *
 * @details    Demonstrates various queue operations using an
 *             implementation provided by "Queue.h".
 */

// Standard library includes
#include <stdio.h>
#include <stdlib.h>

// User-defined includes
#include "Queue.h"

int main(void) {

    // Create and initialize a new Queue buffer
    Queue_Buffer_t* buffer = queue_buffer_create();
    if (!buffer) {
        printf("Error: Failed to initialize the queue buffer.\n");
        return -1;
    }

    // Verify the front of the queue
    if (queue_front(buffer)) {
        printf("Error: Problem retrieving the front of the queue.\n");
    }

    // Populate the queue with data
    printf("Populating the queue...\n");
    for (uint8_t i = 0; i < BUFFER_SIZE + 5; ++i) {
        if (!queue_enqueue(buffer, &i)) {
            printf("Alert: Queue reached its maximum capacity at data point %d.\n", i);
            break;
        }
    }

    // Output the current status of the queue
    printf("Current queue status: Capacity = %d\n", queue_current_capacity(buffer));

    // Extract and display the front elements of the queue
    printf("Extracting front elements from the queue...\n");
    uint8_t dequeuedData;
    for (uint8_t i = 0; i < 20; ++i) {
        queue_dequeue(buffer, &dequeuedData);
        printf("Data point %d: %d\n", i, dequeuedData);
    }

    // Display the remaining capacity of the queue post extraction
    printf("Post extraction queue status: Remaining Capacity = %d\n", queue_current_capacity(buffer));

    // Extract a batch of elements from the queue at once
    uint8_t batchData[100] = {0};
    queue_dequeue_first_x(buffer, batchData, 100);

    // Display the batch extracted data
    printf("Data extracted in batch:\n");
    for (uint8_t i = 0; i < 100; i++) {
        printf("%d ", batchData[i]);
    }
    printf("\n");

    return 0;
}
