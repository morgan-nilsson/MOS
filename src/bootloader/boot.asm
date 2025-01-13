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

    xor ax, ax
    mov ds, ax
    mov es, ax

    mov si, msg
    call puts

    cli
    hlt

times 510-($-$$) db 0
dw 0AA55h
