ENTRY(_start)

SECTIONS {
    . = PHY_KERNEL_START;
    
    .text : {
        *(.text)      /* Code section */
    }

    .data : {
        _data = .;    /* Start of .data section */
        *(.data)      /* Initialized data */
        _edata = .;   /* End of .data section */
    }

    .bss : {
        _bss = .;     /* Start of .bss section */
        *(.bss)       /* Uninitialized data */
        _ebss = .;    /* End of .bss section */
    }

    /DISCARD/ : {
        *(.comment)
    }

    _end = .;         /* End of all sections */
}
