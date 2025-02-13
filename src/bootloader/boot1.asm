org 0x7c00
bits 16
jmp loader

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

msg db "Error", 0
loadMsg db "Loading bootloader2...", 10, 13, 0

puts:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 10h
    jmp puts
.done:
    ret

; Entry point for bootloader
loader:

    mov si, loadMsg
    call puts

    xor ax, ax
    mov es, ax

    mov ah, 0x02	        ; BIOS function read sector
    mov al, 0x02	        ; reading size 2 sector
    mov ch, 0x00            ; 
    mov cl, 0x02		    ; sector = 2
    mov dh, 0x00		    ; head = 0

    mov bx, 0x1000          ; address to read into
    int 0x13		        ; call the bios

    jc boot_error

    jmp 0x1000

boot_error:
    mov si, msg
    call puts

    hlt

times 510-($-$$) db 0
dw 0AA55h