# MOS
My Operating System

This is a simple hobby operating system written in C and Assembly. It is written to target i386. It has a stage 1 and 2 bootloader, a kernel and is designed to run in QEMU.

- Two-stage bootloader (real mode to protected mode, A20 enable)
- Paging with a user/kernel address space split
- GDT with ring 0 and ring 3 segments, TSS
- IDT, CPU exception and IRQ handling
- PIT timer and PS/2 keyboard drivers, VGA text mode
- Syscall interface: read, write, sleep
- Preemptive round-robin scheduler (PIT-driven time slices)
- First-fit heap allocator with block splitting and coalescing
- Minimal libc: string, ctype, math, assert, errno

## Build
```bash
make
```

## Run
```bash
make run
```

## Clean
```bash
make clean
```