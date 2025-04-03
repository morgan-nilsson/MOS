
ENTRY(kernel_entry);
OUTPUT_FORMAT(binary)

SECTIONS {
    . = 0x8000;

    .text.boot : ALIGN(4) {
        KEEP(*(.text.entry))
    }

    /DISCARD/ : {
        *(.comment)
    }

}
