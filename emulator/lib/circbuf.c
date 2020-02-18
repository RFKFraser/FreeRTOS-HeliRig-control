#include <stdint.h>
#include <stdlib.h>
#include "circbuf.h"

// initCircBuf : Initialise the circBuf instance. Reset both indices to
// the start of the buffer.  Dynamically allocate and clear the the
// memory and return a pointer for the data.  Return NULL if
// allocation fails.
uint32_t* initCircBuf(circbuf_t* buffer, uint32_t size)
{
    buffer->windex = 0;
    buffer->rindex = 0;
    buffer->size = size;
    buffer->data = (uint32_t*) calloc(size, sizeof(uint32_t)); // calloc() to clear contents.
    return buffer->data;
}

// writeCircBuf : insert entry at the current windex location, advance windex, modulo (buffer size).
void writeCircBuf(circbuf_t* buffer, uint32_t entry)
{
    buffer->data[buffer->windex++] = entry;
    buffer->windex = buffer->windex >= buffer->size ? 0 : buffer->windex;
}

// readCircBuf : return entry at the current rindex location,
// advance rindex, modulo (buffer size). The function deos not check
// if reading has advanced ahead of writing.
uint32_t readCircBuf(circbuf_t* buffer)
{
    uint32_t entry;

    entry = buffer->data[buffer->rindex++];
    buffer->rindex = buffer->rindex >= buffer->size ? 0 : buffer->rindex;

    return entry;
}

// freeCircBuf : Releases the memory allocated to the buffer data,
// sets pointer to NULL and other fields to 0. The buffer can
// re-initialised by another call to initCircBuf().
void freeCircBuf(circbuf_t* buffer)
{
    buffer->windex = 0;
    buffer->rindex = 0;
    buffer->size = 0;
    free(buffer->data);
    buffer->data = NULL;
}

// readCircBufMean : Returns the mean value of the circular buffer
uint32_t readCircBufMean(circbuf_t* buffer)
{
    uint8_t i;
    uint32_t sum = 0;

    for (i = 0; i < buffer->size; i++)
    {
        sum += readCircBuf(buffer);
    }

    return (2 * sum + buffer->size) / 2 / buffer->size;
}

