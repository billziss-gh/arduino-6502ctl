    ; hello.asm: 6502 hello world

    .def ind0 $fe   ; zero page indirect helper lo
    .def ind1 $ff   ; zero page indirect helper hi

    .org $c000

entry:
    LDA #$ff
    TAX
    TXS             ; S = #$ff: top of stack
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
    LDA $104,X      ; fetch P
    BIT #$10        ; test B bit
    BNE brk

irq_main:

    PLY
    PLX
    PLA
    RTI

brk:
    SEC
    LDA $105,X      ; fetch retaddr lo
    SBC #1
    STA ind0
    LDA $106,X      ; fetch retaddr hi
    SBC #0
    STA ind1
    LDA (ind0)      ; fetch BRK

    PLY
    PLX
    PLA
    RTI

nmi:
    RTI

    .org $fffa

    .word nmi
    .word entry
    .word irq
