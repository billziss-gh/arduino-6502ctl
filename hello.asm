    ; hello.asm: hello world from 6502

    .def IND0 $fe   ; zp indirect lo
    .def IND1 $ff   ; zp indirect hi

    .def IRQN 0     ; mio: IRQ number register
    .def OREG 64    ; mio: output register

    .org $c000

entry:
    LDA #$ff
    TAX
    TXS             ; S = #$ff: top of stack
    LDA #0          ; A = #0
    TAX             ; X = #0
    TAY             ; Y = #0
    CLI

    LDX str
loop:
    LDA str,X
    STA OREG,X
    DEX
    BNE loop
    LDX str
    STX OREG

done:
    BRA done

str:
    .byte "hello world from 6502\n"

irq:
    PHA
    PHX
    PHY

    TSX
    LDA $104,X      ; fetch P
    BIT #$10        ; test B bit
    BNE brk

    LDA IRQN        ; A = IRQ number

irq_exit:
    LDA #0
    STA IRQN        ; clear IRQ

    PLY
    PLX
    PLA
    RTI

brk:
    SEC
    LDA $105,X      ; fetch retaddr lo
    SBC #1
    STA IND0
    LDA $106,X      ; fetch retaddr hi
    SBC #0
    STA IND1
    LDA (IND0)      ; fetch BRK

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
