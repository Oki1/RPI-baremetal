.section ".text.boot"  //put this the start of the kernel image

.global _start  //execute from here

_start:

    //check if main core
    MRC p15, 0, r1, c0, c0, 5
    AND r1, r1, #0x3 //extract cpu id bits
    CMP r1, #0
    BEQ   2f
1:  wfe
    b   1b
2:  ldr     x1, =_start
    mov     sp, x1

    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
3:  cbz     w2, 4f               // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b               // Loop if non-zero

    // Jump to our main() routine in C (make sure it doesn't return)
4:  bl      main
    // In case it does return, halt the master core too
    b       1b