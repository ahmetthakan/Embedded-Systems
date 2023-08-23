/*
 ============================================================================
 * @file       main.c
 * @author    Ekrem Demir
 * @version    1.0
 * @date       23-08-2023
 * @brief      Stack operations demonstration program.
 *
 * @details    This program showcases various stack operations using
 *             an implementation from "Stack.h".
 ============================================================================
 */


#include <stdio.h>
#include "Stack.h"

int main() {
    // Create a new stack buffer
    Stack_Buffer_t* buffer = stack_buffer_create();
    if (!buffer) {
        printf("Failed to create stack buffer.\n");
        return -1;
    }

    // Push data onto the stack
    printf("Pushing data onto the stack...\n");
    for (uint8_t i = 0; i < (BUFFER_SIZE + 5); ++i) {
        if (!stack_push(buffer, &i)) {
            printf("Buffer reached max capacity at iteration %d!\n", i);
            break;
        }
    }
    printf("Current stack capacity: %d\n", stack_current_capacity(buffer));

    // Pop data from the stack and display it
    printf("Popping data from the stack...\n");
    uint8_t popData;
    for (uint8_t i = 0; i < 20; ++i) {
        if (stack_pop(buffer, &popData)) {
            printf("Popped data: %d\n", popData);
        }
    }
    printf("Current stack capacity after popping: %d\n", stack_current_capacity(buffer));

    // Pop multiple elements from the stack
    uint8_t batchData[20];
    stack_pop_first_x(buffer, batchData, sizeof(batchData) / sizeof(batchData[0]));

    // Print the batch-popped data
    printf("Batch-popped data:\n");
    for (int i = 0; i < sizeof(batchData) / sizeof(batchData[0]); i++) {
        printf("%d ", batchData[i]);
    }
    printf("\n");
    printf("Current stack capacity after batch-popping: %d\n", stack_current_capacity(buffer));

    // Cleanup: Reset and destroy the stack buffer
    stack_reset(buffer);
    stack_buffer_destroy(buffer);

    return 0;
}
