/**
 * @file       Queue.c
 * @author     Ekrem Demir
 * @version    1.0
 * @date       23-08-2023
 * @brief      Queue operations implementation.
 *
 * @details    Provides functions for various queue operations using a circular
 *             buffer implementation.
 */

// Standard library includes
#include <stdlib.h>
#include <string.h>

// User-defined includes
#include "Queue.h"

// Private function prototypes


// Create a new queue buffer and initialize it
Queue_Buffer_t* queue_buffer_create() {
    Queue_Buffer_t *buffer = (Queue_Buffer_t*)malloc(sizeof(Queue_Buffer_t));
    if (buffer) {
        queue_init(buffer);
    }
    return buffer;
}


// Free up allocated space for the queue buffer
void queue_buffer_destroy(Queue_Buffer_t *buffer) {
    free(buffer);
}

void queue_init(Queue_Buffer_t *buffer)
{
	buffer->front = 0;
	buffer->rear = 0;
}

// Reset the queue buffer pointers
void queue_reset(Queue_Buffer_t *buffer) {
    buffer->front = 0;
    buffer->rear = 0;
}



// Returns the current number of elements in the queue
uint8_t queue_current_capacity(Queue_Buffer_t *buffer) {
    if (queue_is_empty(buffer)) {
        return 0;
    }
    if (buffer->rear >= buffer->front) {
        return (buffer->rear - buffer->front + 1);
    }
    return (BUFFER_SIZE + buffer->rear - buffer->front + 1);
}

// Check if queue is full
uint8_t queue_is_full(Queue_Buffer_t *buffer) {
    return (((buffer->front + 1) % BUFFER_SIZE) == buffer->rear);
}

// Check if queue is empty
uint8_t queue_is_empty(Queue_Buffer_t *buffer) {
    return (buffer->front == buffer->rear);
}

// Return the front element of the queue
uint8_t queue_front(Queue_Buffer_t *buffer) {
    if (queue_is_empty(buffer)) {
        return 0;  // Consider using a different error value or mechanism
    }
    return buffer->data[buffer->rear];
}

// Add an element to the queue
uint8_t queue_enqueue(Queue_Buffer_t *buffer, uint8_t *data) {
    if (queue_is_full(buffer)) {
        return 0;
    }
    buffer->data[buffer->front++] = *data;
    buffer->front %= BUFFER_SIZE;
    return 1;
}

// Remove the front element from the queue
uint8_t queue_dequeue(Queue_Buffer_t *buffer, uint8_t *data) {
    if (queue_is_empty(buffer)) {
        return 0;
    }
    *data = buffer->data[buffer->rear++];
    buffer->rear %= BUFFER_SIZE;
    return 1;
}

// Dequeue multiple elements from the front of the queue
uint8_t queue_dequeue_first_x(Queue_Buffer_t *buffer, uint8_t *data, uint8_t len_data_to_be_read) {
    if (queue_is_empty(buffer)) {
        return 0;
    }
    uint8_t current_capacity = queue_current_capacity(buffer);
    if (len_data_to_be_read > current_capacity) {
        len_data_to_be_read = current_capacity;
    }
    for (uint8_t i = 0; i < len_data_to_be_read; ++i) {
        *(data + i) = buffer->data[buffer->rear++];
        buffer->rear %= BUFFER_SIZE;
    }
    return 1;
}
