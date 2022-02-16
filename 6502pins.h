/*
 * 6502pins.h
 * 6502 pin to Arduino pin mapping
 *
 * Copyright 2022 Bill Zissimopoulos
 */

/* 6502 address bus pins (ATmega2560 PF0-7, PK0-7) */
#define PIN6502_A0                      54
#define PIN6502_A1                      55
#define PIN6502_A2                      56
#define PIN6502_A3                      57
#define PIN6502_A4                      58
#define PIN6502_A5                      59
#define PIN6502_A6                      60
#define PIN6502_A7                      61
#define PIN6502_A8                      62
#define PIN6502_A9                      63
#define PIN6502_A10                     64
#define PIN6502_A11                     65
#define PIN6502_A12                     66
#define PIN6502_A13                     67
#define PIN6502_A14                     68
#define PIN6502_A15                     69
#define P6502_ABLO(n)                   n ## F
#define P6502_ABHI(n)                   n ## K

/* 6502 data bus pins (ATmega2560 PL0-7) */
#define PIN6502_D0                      49
#define PIN6502_D1                      48
#define PIN6502_D2                      47
#define PIN6502_D3                      46
#define PIN6502_D4                      45
#define PIN6502_D5                      44
#define PIN6502_D6                      43
#define PIN6502_D7                      42
#define P6502_DBUS(n)                   n ## L

/* 6502 input control pins (ATmega2560 PA0-6) */
#define PIN6502_RDY                     22
#define PIN6502_IRQB                    23
#define PIN6502_NMIB                    24
#define PIN6502_RESB                    25
#define PIN6502_SOB                     26
#define PIN6502_PHI2                    27
#define PIN6502_BE                      28
#define PIN6502_ICTL_UNUSED7            29
#define P6502_ICTL(n)                   n ## A
#define P6502_ICTL_PIN(n)               (1 << (PIN6502_ ## n - PIN6502_RDY))

/* 6502 output control pins (ATmega2560 PC2-7) */
#define PIN6502_OCTL_UNUSED0            37
#define PIN6502_OCTL_UNUSED1            36
#define PIN6502_RWB                     35
#define PIN6502_PHI2O                   34
#define PIN6502_SYNC                    33
#define PIN6502_MLB                     32
#define PIN6502_PHI1O                   31
#define PIN6502_VPB                     30
#define P6502_OCTL(n)                   n ## C
#define P6502_OCTL_PIN(n)               (1 << (PIN6502_OCTL_UNUSED0 - PIN6502_ ## n))
