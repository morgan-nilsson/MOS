org 0x1000
bits 16
jmp main

boot2Loaded db "bootloader2 loaded", 10, 13, 0
msg db "preparing to load operating system...", 10, 13, 0

puts:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 10h
    jmp puts
.done:
    ret

main:
    cli		
    push cs
    pop ds

    mov esi, boot2Loaded
    call puts

    cli
    hlt