    ; hello.as: 6502 hello world

    .org $c000

entry:
    LDA #$ff
    TAX
    TXS             ; S = #$ff ; top of stack
    LDA #0          ; A = #0
    TAX             ; X = #0
    TAY             ; Y = #0
    CLI

main:
    BRK #$42
    BRA main

irq:
    PHA
    PHX
    PHY

    TSX
    LDA $04,X       ; fetch P
    BIT #$10        ; test B bit
    BNE brk

irq_main:

    PLY
    PLX
    PLA
    RTI

brk:
    LDA ($05,X)     ; fetch BRK

    PLY
    PLX
    PLA
    RTI

nmi:
    RTI

    .org $fffa

    .word irq
    .word entry
    .word nmi
