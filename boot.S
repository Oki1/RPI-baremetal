.section ".text.boot"

.global _start

_start:
loop:  wfe //just rest all cores
    b loop