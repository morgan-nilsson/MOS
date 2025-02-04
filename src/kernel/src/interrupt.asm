[extern isr_handler]
[extern irq_handler]

irs_common_stub:

    pusha           ; push gen regs

    mov ax, ds
    push eax        ; push ds selector

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; give stack back to c
    push esp

    call isr_handler

    pop eax

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa

    ; remove int_no and err_code 8 bytes
    add esp, 8

    iret

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31


; 0: divide by zero
isr0:
    push byte 0
    push byte 0
    jmp irs_common_stub

; 1: debug
isr1:
    push byte 0
    push byte 1
    jmp irs_common_stub

; 2: NMI
isr2:
    push byte 0
    push byte 2
    jmp irs_common_stub

; 3: breakpoint
isr3:
    push byte 0
    push byte 3
    jmp irs_common_stub

; 4: overflow
isr4:
    push byte 0
    push byte 4
    jmp irs_common_stub

; 5: bound range exceeded
isr5:
    push byte 0
    push byte 5
    jmp irs_common_stub

; 6: invalid opcode
isr6:
    push byte 0
    push byte 6
    jmp irs_common_stub

; 7: device not available
isr7:
    push byte 0
    push byte 7
    jmp irs_common_stub

; 8: double fault
isr8:
    push byte 0
    push byte 8
    jmp irs_common_stub

; 9: coprocessor segment overrun
isr9:
    push byte 0
    push byte 9
    jmp irs_common_stub

; 10: invalid TSS
isr10:
    push byte 0
    push byte 10
    jmp irs_common_stub

; 11: segment not present
isr11:
    push byte 0
    push byte 11
    jmp irs_common_stub

; 12: stack segment fault
isr12:
    push byte 0
    push byte 12
    jmp irs_common_stub

; 13: general protection fault
isr13:
    push byte 0
    push byte 13
    jmp irs_common_stub

; 14: page fault
isr14:
    push byte 0
    push byte 14
    jmp irs_common_stub

; 15: reserved
isr15:
    push byte 0
    push byte 15
    jmp irs_common_stub

; 16: x87 FPU error
isr16:
    push byte 0
    push byte 16
    jmp irs_common_stub

; 17: alignment check
isr17:
    push byte 0
    push byte 17
    jmp irs_common_stub

; 18: machine check
isr18:
    push byte 0
    push byte 18
    jmp irs_common_stub

; 19: SIMD floating point exception
isr19:
    push byte 0
    push byte 19
    jmp irs_common_stub

; 20: virtualization exception
isr20:
    push byte 0
    push byte 20
    jmp irs_common_stub

; 21: reserved
isr21:
    push byte 0
    push byte 21
    jmp irs_common_stub

; 22: reserved
isr22:
    push byte 0
    push byte 22
    jmp irs_common_stub

; 23: reserved
isr23:
    push byte 0
    push byte 23
    jmp irs_common_stub

; 24: reserved
isr24:
    push byte 0
    push byte 24
    jmp irs_common_stub

; 25: reserved
isr25:
    push byte 0
    push byte 25
    jmp irs_common_stub

; 26: reserved
isr26:
    push byte 0
    push byte 26
    jmp irs_common_stub

; 27: reserved
isr27:
    push byte 0
    push byte 27
    jmp irs_common_stub

; 28: reserved
isr28:
    push byte 0
    push byte 28
    jmp irs_common_stub

; 29: reserved
isr29:
    push byte 0
    push byte 29
    jmp irs_common_stub

; 30: reserved
isr30:
    push byte 0
    push byte 30
    jmp irs_common_stub

; 31: reserved
isr31:
    push byte 0
    push byte 31
    jmp irs_common_stub

irq_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
    pop ebx

    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    iret

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15


irq0:
    push byte 0
    push byte 32
    jmp irq_common_stub

irq1:
    push byte 1
    push byte 33
    jmp irq_common_stub

irq2:
    push byte 2
    push byte 34
    jmp irq_common_stub

irq3:
    push byte 3
    push byte 35
    jmp irq_common_stub

irq4:
    push byte 4
    push byte 36
    jmp irq_common_stub

irq5:
    push byte 5
    push byte 37
    jmp irq_common_stub

irq6:
    push byte 6
    push byte 38
    jmp irq_common_stub 

irq7:
    push byte 7
    push byte 39
    jmp irq_common_stub

irq8:
    push byte 8
    push byte 40
    jmp irq_common_stub

irq9:
    push byte 9
    push byte 41
    jmp irq_common_stub

irq10:
    push byte 10
    push byte 42
    jmp irq_common_stub

irq11:
    push byte 11
    push byte 43
    jmp irq_common_stub

irq12:
    push byte 12
    push byte 44
    jmp irq_common_stub

irq13:
    push byte 13
    push byte 45
    jmp irq_common_stub

irq14:
    push byte 14
    push byte 46
    jmp irq_common_stub

irq15:
    push byte 15
    push byte 47
    jmp irq_common_stub