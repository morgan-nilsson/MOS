#ifndef REFCOUNT_H
#define REFCOUNT_H

#include "libs/stdtypes.h"

/**
 * A simple reference counting structure.
 */
typedef struct rc {
    uint32_t count;
    void* ptr;
} rc_t;

/**
 * Creates a new reference counting structure for the given pointer.
 */
rc_t* rc_new(rc_t* rc, void* ptr);

/**
 * Clones the reference counting structure, increasing the reference count.
 */
rc_t* rc_clone(rc_t* rc);

/**
 * Drops a reference, decreasing the reference count.
 * If the count reaches zero, the pointed-to memory and the rc_t structure are freed.
 */
void rc_drop(rc_t* rc);

/**
 * Returns the current reference count.
 */
uint32_t rc_count(rc_t* rc);


#endif