#ifndef PAGING_H
#define PAGING_H

#include "libs/stdtypes.h"

typedef uint32_t page_dir_entry_t;
typedef uint32_t page_table_entry_t;
typedef uint32_t virtual_addr_t;
typedef uint32_t physical_addr_t;

void init_paging();

#endif