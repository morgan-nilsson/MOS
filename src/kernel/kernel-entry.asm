; Copyright 2025 Morgan Nilsson

section .text.entry
global kernel_entry
[bits 32]
[extern _start]
kernel_entry:
    call _start
    hlt
