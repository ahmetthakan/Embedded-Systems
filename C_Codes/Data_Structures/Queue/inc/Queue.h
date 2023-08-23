/**
 ============================================================================================
 * @file       Queue.h
 * @author     Ekrem Demir
 * @version    1.0
 * @date       23-08-2023
 * @brief      Header file for queue operations.
 *
 * @details    This file provides the data structures and function prototypes
 *             for manipulating a queue based on a circular buffer implementation.
 ============================================================================================
 */

#ifndef QUEUE_H_
#define QUEUE_H_

// Standard includes
#include <stdint.h>

// Define the maximum size of the queue buffer
#define BUFFER_SIZE 100

// Define the data structure for the queue buffer
typedef struct {
    uint8_t data[BUFFER_SIZE];
    uint8_t front;
    uint8_t rear;
} Queue_Buffer_t;

// Function prototypes for queue operations

// Memory management
Queue_Buffer_t* queue_buffer_create();
void queue_buffer_destroy(Queue_Buffer_t *buffer);

// Initialization and reset
void queue_reset(Queue_Buffer_t *buffer);
void queue_init(Queue_Buffer_t *buffer);

// Information retrieval
uint8_t queue_current_capacity(Queue_Buffer_t *buffer);
uint8_t queue_front(Queue_Buffer_t *buffer);
uint8_t queue_is_full(Queue_Buffer_t *buffer);
uint8_t queue_is_empty(Queue_Buffer_t *buffer);

// Data manipulation
uint8_t queue_enqueue(Queue_Buffer_t *buffer, uint8_t *data);
uint8_t queue_dequeue(Queue_Buffer_t *buffer, uint8_t *data);
uint8_t queue_dequeue_first_x(Queue_Buffer_t *buffer, uint8_t *data, uint8_t len_data_to_be_read);

#endif /* QUEUE_H_ */
