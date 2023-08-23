/**
 ==========================================================================================
 * @file       Stack.c
 * @author     Ekrem Demir
 * @version    1.0
 * @date       23-08-2023
 * @brief      Stack operations implementation.
 *
 * Description: This file provides the functionality for the stack operations
 *              used in the main demonstration program.
 ===========================================================================================
 */

#include <stdlib.h>
#include <string.h>
#include "Stack.h"

/**
 * @brief      Create and initialize a new stack buffer.
 * @return     Pointer to the newly created stack buffer or NULL if allocation failed.
 */
Stack_Buffer_t* stack_buffer_create() {
    Stack_Buffer_t* buffer = (Stack_Buffer_t*)malloc(sizeof(Stack_Buffer_t));
    if (buffer) {
        stack_init(buffer);
    }
    return buffer;
}

/**
 * @brief      Frees the allocated memory for the stack buffer.
 * @param      buffer   Pointer to the stack buffer.
 */
void stack_buffer_destroy(Stack_Buffer_t* buffer) {
    if (buffer) {
        free(buffer);
    }
}

/**
 * @brief      Initializes the stack buffer.
 * @param      buffer   Pointer to the stack buffer.
 */
void stack_init(Stack_Buffer_t* buffer) {
    buffer->index = 0;
}

/**
 * @brief      Resets the stack buffer.
 * @param      buffer   Pointer to the stack buffer.
 */
void stack_reset(Stack_Buffer_t* buffer) {
    buffer->index = 0;
}

/**
 * @brief      Retrieves the current number of elements in the stack buffer.
 * @param      buffer   Pointer to the stack buffer.
 * @return     Number of elements in the stack.
 */
uint8_t stack_current_capacity(Stack_Buffer_t* buffer) {
    return buffer->index;
}

/**
 * @brief      Gets the top element from the stack without removing it.
 * @param      buffer   Pointer to the stack buffer.
 * @return     The top element of the stack.
 */
uint8_t stack_peek(Stack_Buffer_t* buffer) {
    if (buffer->index == 0)
        return 0; // or another invalid value or error code
    return buffer->data[buffer->index - 1];
}

/**
 * @brief      Checks if the stack buffer is full.
 * @param      buffer   Pointer to the stack buffer.
 * @return     1 if full, 0 otherwise.
 */
uint8_t stack_is_full(Stack_Buffer_t* buffer) {
    return (buffer->index >= BUFFER_SIZE);
}

/**
 * @brief      Checks if the stack buffer is empty.
 * @param      buffer   Pointer to the stack buffer.
 * @return     1 if empty, 0 otherwise.
 */
uint8_t stack_is_empty(Stack_Buffer_t* buffer) {
    return (buffer->index == 0);
}

/**
 * @brief      Pushes a data element onto the stack.
 * @param      buffer   Pointer to the stack buffer.
 * @param      data     The data to be pushed.
 * @return     1 if successful, 0 otherwise.
 */
uint8_t stack_push(Stack_Buffer_t* buffer, uint8_t *data) {
    if(!data || stack_is_full(buffer)) {
        return 0;
    }
    buffer->data[buffer->index++] = *data;
    return 1;
}

/**
 * @brief      Pops a data element from the stack.
 * @param      buffer   Pointer to the stack buffer.
 * @param      data     Pointer to store the popped data.
 * @return     1 if successful, 0 otherwise.
 */
uint8_t stack_pop(Stack_Buffer_t* buffer, uint8_t *data) {
    if(!data || stack_is_empty(buffer)) {
        return 0;
    }
    *data = buffer->data[--buffer->index];
    return 1;
}

/**
 * @brief      Pops the first X data elements from the stack.
 * @param      buffer              Pointer to the stack buffer.
 * @param      data                The array to store the popped data.
 * @param      len_data_to_be_read Number of elements to pop.
 * @return     1 if successful, 0 otherwise.
 */
uint8_t stack_pop_first_x(Stack_Buffer_t* buffer, uint8_t *data, uint8_t len_data_to_be_read) {
    if(!data || stack_is_empty(buffer)) {
        return 0;
    }

    if(len_data_to_be_read < buffer->index) {
        len_data_to_be_read = buffer->index;
    }

    for(uint8_t i = 0; i < len_data_to_be_read; ++i) {
        data[i] = buffer->data[buffer->index - i];
    }

    memmove(buffer->data, buffer->data + len_data_to_be_read, buffer->index - len_data_to_be_read);
    buffer->index -= len_data_to_be_read;

    return 1;
}
