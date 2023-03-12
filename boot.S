.section ".text.boot"

.global _start

_start:
    //read cpu id, continue if main core
    mrs x1, mpidr_el1
    and x1, x1, #3
    cbz x1, maincore
rest:  wfe
    b       rest
maincore: // cpu id = 0
    // set stack to above code
    ldr     x1, =_start
    mov     sp, x1

    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
clearbss:  cbz     w2, mainfunc
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, clearbss

    // jump to c
mainfunc:  bl      main
    // failsafe
    b       rest