#ifndef VBE_DRIVER_H
#define VBE_DRIVER_H

#include "../../libs/include/stdtypes.h"

// https://wiki.osdev.org/VESA_Video_Modes
struct VbeInfoBlock {
   char     VbeSignature[4];         // == "VESA"
   uint16_t VbeVersion;              // == 0x0300 for VBE 3.0
   uint16_t OemStringPtr[2];         // isa vbeFarPtr
   uint8_t  Capabilities[4];
   uint16_t VideoModePtr[2];         // isa vbeFarPtr
   uint16_t TotalMemory;             // as # of 64KB blocks
   uint8_t  Reserved[492];
} __attribute__((packed));

void installVBE();

#endif