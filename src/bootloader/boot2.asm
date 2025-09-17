; Copyright 2025 Morgan Nilsson
org 0x1000
bits 16
jmp main

msgKernelFailed db "Unable to read kernel to memory", 10, 13, 0
enteredProtectedMode db "Entered protected mode", 0
vbe3 db "VBE 3.0", 0
vbe2 db "VBE 2.0", 0
vbeUnsuported db "VBE unsupported", 0
VBEError db "VBE call failed", 0
debug db "Debug", 0
a20ns db "A20 not supported", 0
a20failed db "A20 activation failed", 0

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

; load kernel into memery at 0x8000
loadKernel: 
    xor ax, ax
    mov es, ax

    mov ah, 0x02
    mov al, 0x80        ; reading 0x80 sectors
    mov ch, 0x00
    mov cl, 0x04        ; sector = 4
    mov dh, 0x00        ; head = 0

    mov bx, 0x8000
    int 0x13

    jc kernel_load_error

    call getVbeInfo

    cmp word [VesaInfoBlockBuffer + VesaInfoBlock.Version], 0x0200
    jne is_30
    mov si, vbe2
    call puts
    jmp supportedVbe
is_30:
    cmp word [VesaInfoBlockBuffer + VesaInfoBlock.Version], 0x0300
    jne unsuported
    mov si, vbe3
    call puts
    jmp supportedVbe
unsuported:
    mov si, vbeUnsuported
    call puts
    hlt
supportedVbe:

    jmp VBEAfter

struc VesaInfoBlock				;	VesaInfoBlock_size = 512 bytes
	.Signature		resb 4		;	must be 'VESA'
	.Version		resw 1
	.OEMNamePtr		resd 1
	.Capabilities		resd 1

	.VideoModesOffset	resw 1
	.VideoModesSegment	resw 1

	.CountOf64KBlocks	resw 1
	.OEMSoftwareRevision	resw 1
	.OEMVendorNamePtr	resd 1
	.OEMProductNamePtr	resd 1
	.OEMProductRevisionPtr	resd 1
	.Reserved		resb 222
	.OEMData		resb 256
endstruc

ALIGN(4)

	VesaInfoBlockBuffer: istruc VesaInfoBlock
		at VesaInfoBlock.Signature,				db "VESA"
		times 508 db 0
	iend

getVbeInfo:
    ; move BVE_INFO to the block
    mov ax, 0x4F00
    mov di, VesaInfoBlockBuffer
    int 0x10

    ; check if the call was successful
    cmp ax, 0x004F
    jne .failed
    ret
.failed:
    mov si, VBEError
    call puts
    ret

VBEAfter:

use16
mov     ax,2403h                ;--- A20-Gate Support ---
int     15h
jb      a20_ns                  ;INT 15h is not supported
cmp     ah,0
jnz     a20_ns                  ;INT 15h is not supported

mov     ax,2402h                ;--- A20-Gate Status ---
int     15h
jb      a20_failed              ;couldn't get status
cmp     ah,0
jnz     a20_failed              ;couldn't get status

cmp     al,1
jz      a20_activated           ;A20 is already activated

mov     ax,2401h                ;--- A20-Gate Activate ---
int     15h
jb      a20_failed              ;couldn't activate the gate
cmp     ah,0
jnz     a20_failed              ;couldn't activate the gate

a20_ns:
    mov si, a20ns
    call puts
    hlt

a20_failed:
    mov si, a20failed
    call puts
    hlt

a20_activated:

cli
lgdt [GDT_Desc]

mov eax, cr0
or eax, 1
mov cr0, eax

jmp 0x08:start_protected_mode

GDT_Start:
    null_desc:
        dd 0
        dd 0
    kernel_code_desc:
        dw 0xffff
        dw 0
        db 0
        db 0x9a             ;10011010
        db 0xcf             ;11001111
        db 0
    kernel_data_desc:
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
    mov sp, 0x9FC00

    jmp enterKernel
    mov edx, enteredProtectedMode
    mov esi, 0xb8000

enterKernel:

    cli
    jmp 0x8000

    hlt