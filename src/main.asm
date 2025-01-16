org 0x7c00
bits 16
start: jmp loader

bpmOEM                  db "My os    "

bpbBytesPerSector:  	DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	    DB 2
bpbRootEntries: 	    DW 224
bpbTotalSectors: 	    DW 2880
bpbMedia: 	            DB 0xF0
bpbSectorsPerFAT: 	    DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	    DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0
bsUnused: 	            DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	        DD 0xa0a1a2a3
bsVolumeLabel: 	        DB "MOS FLOPPY "
bsFileSystem: 	        DB "FAT12   "

msg db "Welcome", 0

;==========================================
;   Print string
    ;   ds=>si: null termanating string
;==========================================

puts:
    lodsb
    or al, al
    jz .done
    mov ah, 0eh
    int 10h
    jmp puts

.done:
    ret

;==========================================
;       Entry point for bootloader
;==========================================
loader:

.Reset:
    mov ah, 0		; reset floppy function
    mov dl, 0		; drive 0
    int 0x13		; call BIOS
    jc .Reset		; if carry flag retry

    mov ax, 0x1000	; we are reading 0x1000
    mov es, ax
    xor bx, bx

    mov ah, 0x02	; read sector
    mov al, 1		; reading size 1 sector
    mov ch, 1		; track = 1
    mov cl, 2		; sector = 2
    mov dh, 0		; head = 0
    mov dl, 0		; drive zero ie from floppy
    int 0x13		; call the bios

    mov si, msg
    call puts

    cli
    hlt

times 510-($-$$) db 0
dw 0AA55h

cli
hlt
