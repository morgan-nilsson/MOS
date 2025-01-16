ENTRY(_start)

SECTIONS {
    . = 1M;  /* Start Stage 2 at 1MB (after Stage 1) */
    
    .text : {
        *(.text)   /* Put all code in the .text section here */
    }

    .data : {
        *(.data)   /* Initialized data section */
    }

    .bss : {
        *(.bss)    /* Uninitialized data section */
    }
    /* Linker Script for Bare-Metal 32-bit x86 System */

    /DISCARD/ : {
        *(.comment)  /* Discard any comments or unwanted sections */
    }
}
