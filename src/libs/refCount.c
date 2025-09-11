#include "libs/refCount.h"
#include "libs/stdlib.h"

rc_t* refc_new(rc_t* rc, void* ptr) {
    if (!rc) {
        return NULL;
    }
    rc->count = 1;
    rc->ptr = ptr;
    return rc;
}

rc_t* refc_clone(rc_t* rc) {
    if (!rc) {
        return NULL;
    }
    rc->count++;
    return rc;
}

void refc_drop(rc_t* rc) {
    if (!rc) {
        return;
    }
    if (rc->count > 0) {
        rc->count--;
        if (rc->count == 0) {
            if (rc->ptr) {
                free(rc->ptr);
            }
            free(rc);
        }
    }
}

uint32_t refc_count(rc_t* rc) {
    if (!rc) {
        return 0;
    }
    return rc->count;
}