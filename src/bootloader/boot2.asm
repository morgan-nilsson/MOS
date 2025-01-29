org 0x1000
bits 16
jmp main

boot2Loaded db "bootloader2 loaded", 10, 13, 0
msgKernelFailed db "Unable to read kernel to memory", 10, 13, 0
enteredProtectedMode db "Entered protected mode", 0

puts:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 10h
    jmp puts
.done:
    ret

kernel_load_error:
    mov si, msgKernelFailed
    call puts
    hlt

main:
    cli		
    push cs
    pop ds

    mov ah, 0x0
    mov al, 0x3
    int 0x10

; load kernel into memry at 0x2000
loadKernel: 
    xor ax, ax
    mov es, ax

    mov ah, 0x02
    mov al, 0x02
    mov ch, 0x00
    mov cl, 0x04        ; sector = 4
    mov dh, 0x00        ; head = 0

    mov bx, 0x2000
    int 0x13

    jc kernel_load_error

cli
lgdt [GDT_Desc]

mov eax, cr0
or eax, 1
mov cr0, eax

jmp 0x08:start_protected_mode

GDT_Start:      ; flat mode
    null_desc:
        dd 0
        dd 0
    code_desc:
        dw 0xffff
        dw 0
        db 0
        db 0x9a             ;10011010
        db 0xcf             ;11001111
        db 0
    data_desc:
        dw 0xffff
        dw 0
        db 0
        db 0x92             ;10010010
        db 0xcf             ;11001111
        db 0
    GDT_End:

GDT_Desc:
    dw GDT_End - GDT_Start - 1  ; size
    dd GDT_Start                ; start

GDT_AFTER:

bits 32
start_protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ax, 0x08
    mov cs, ax

    mov ax, 0x10
    mov ss, ax
    mov sp, 0x7c00

    jmp enterKernel
    mov edx, enteredProtectedMode
    mov esi, 0xb8000

protectedModeMsg:

    mov al, [edx]
    or al, al
    jz enterKernel
    mov [esi], al

    add esi, 2
    add edx, 1

    jmp protectedModeMsg

enterKernel:

    cli
    call 0x2000

    hlt