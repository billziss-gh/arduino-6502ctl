    ; hello.as: 6502 hello world

    .org $c000

    LDA #1
    STA 0

    .org $fffa

    .word $0000
    .word $c000
    .word $0000
