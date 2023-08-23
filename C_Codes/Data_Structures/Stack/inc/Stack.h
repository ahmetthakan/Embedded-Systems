/**
  ===========================================================================================
 * @file       Stack.h
 * @author     Ekrem Demir
 * @version    1.0
 * @date       23-08-2023
 * @brief      Stack data structure and its operations.
 *
 * Description: This header defines the stack data structure and its associated
 *              operations to be used in the main program.
  ===========================================================================================
 */

#ifndef STACK_H_
#define STACK_H_

#include <stdint.h>

// Constants
#define BUFFER_SIZE 100

// Data structures
/**
 * @brief Stack data structure
 *
 * The Stack_Buffer_t structure contains the actual stack data and its current index.
 */
typedef struct {
    uint8_t data[BUFFER_SIZE];  // Stack data
    uint8_t index;              // Current index (top of the stack)
} Stack_Buffer_t;

// Function prototypes

/**
 * @brief Create and initialize a new stack buffer.
 * @return Pointer to the newly created stack buffer or NULL if allocation failed.
 */
Stack_Buffer_t* stack_buffer_create();

/**
 * @brief Frees the allocated memory for the stack buffer.
 * @param buffer Pointer to the stack buffer.
 */
void stack_buffer_destroy(Stack_Buffer_t* buffer);

/**
 * @brief Initializes the stack buffer.
 * @param buffer Pointer to the stack buffer.
 */
void stack_init(Stack_Buffer_t* buffer);

/**
 * @brief Resets the stack buffer.
 * @param buffer Pointer to the stack buffer.
 */
void stack_reset(Stack_Buffer_t* buffer);

/**
 * @brief Retrieves the current number of elements in the stack buffer.
 * @param buffer Pointer to the stack buffer.
 * @return Number of elements in the stack.
 */
uint8_t stack_current_capacity(Stack_Buffer_t* buffer);

/**
 * @brief Gets the top element from the stack without removing it.
 * @param buffer Pointer to the stack buffer.
 * @return The top element of the stack.
 */
uint8_t stack_peek(Stack_Buffer_t* buffer);

/**
 * @brief Checks if the stack buffer is full.
 * @param buffer Pointer to the stack buffer.
 * @return 1 if full, 0 otherwise.
 */
uint8_t stack_is_full(Stack_Buffer_t* buffer);

/**
 * @brief Checks if the stack buffer is empty.
 * @param buffer Pointer to the stack buffer.
 * @return 1 if empty, 0 otherwise.
 */
uint8_t stack_is_empty(Stack_Buffer_t* buffer);

/**
 * @brief Pushes a data element onto the stack.
 * @param buffer Pointer to the stack buffer.
 * @param data The data to be pushed.
 * @return 1 if successful, 0 otherwise.
 */
uint8_t stack_push(Stack_Buffer_t* buffer, uint8_t* data);

/**
 * @brief Pops a data element from the stack.
 * @param buffer Pointer to the stack buffer.
 * @param data Pointer to store the popped data.
 * @return 1 if successful, 0 otherwise.
 */
uint8_t stack_pop(Stack_Buffer_t* buffer, uint8_t* data);

/**
 * @brief Pops the first X data elements from the stack.
 * @param buffer Pointer to the stack buffer.
 * @param data The array to store the popped data.
 * @param len_data_to_be_read Number of elements to pop.
 * @return 1 if successful, 0 otherwise.
 */
uint8_t stack_pop_first_x(Stack_Buffer_t* buffer, uint8_t* data, uint8_t len_data_to_be_read);

#endif /* STACK_H_ */
