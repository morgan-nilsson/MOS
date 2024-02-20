org 0x7c00
bits 16

%define ENDL 0x0D,0x0A

start:
    jmp main

puts:           ; print a string from register si

    push si     ; save the values in there registers to the stack                    
    push ax
    push bx

.loop:
    lodsb       ; load a byte from the SI register to the al register
    or si, si   ; check if null(0) is the next value in the string
    jz .done

    mov ah, 0x0E; print the character
    mov bh, 0   ; page zero
    int 0x10

    jmp .loop

.done:
    pop bx
    pop ax
    pop si
    ret

main:
    mov ax, 0       ; set up the data segs
    mov ds, ax
    mov es, ax
    
    mov ss, ax      ; stack start 
    mov sp, 0x7C00  ; set stack pointer to largest size
                    ; stack will grow downwords

    mov si, hello
    call puts

    hlt

.halt:
    jmp .halt

hello: db 'Hello World', ENDL, 0

times 510-($-$$) db 0
dw 0AA55h