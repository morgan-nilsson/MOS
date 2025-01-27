ENTRY(_start)

SECTIONS {
    . = 0x2000;
    
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
