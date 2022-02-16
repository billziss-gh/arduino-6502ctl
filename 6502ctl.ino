/*
 * 6502ctl.ino
 * 6502 controller
 *
 * W65C02S
 * https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf
 *
 * Copyright 2022 Bill Zissimopoulos
 */

#include "6502pins.h"

#define SERIAL_SPEED                    1000000

/* 6502 address bus */
static inline void setup_abus()
{
    /* setup ATmega ports designated for 6502 address bus (ABLO, ABHI) for input */
    cli();
    P6502_ABLO(DDR) = 0;
    P6502_ABLO(PORT) = 0;
    P6502_ABHI(DDR) = 0;
    P6502_ABHI(PORT) = 0;
    sei();
}
static inline uint16_t read_abus()
{
    /* read ATmega ports designated for 6502 address bus (ABLO, ABHI) */
    return P6502_ABLO(PIN) | (P6502_ABHI(PIN) << 8);
}

/* 6502 data bus */
static inline void setup_dbus()
{
    /* setup ATmega port designated for 6502 data bus (DBUS) for input */
    cli();
    P6502_DBUS(DDR) = 0;
    P6502_DBUS(PORT) = 0;
    sei();
}
static inline uint8_t read_dbus()
{
    /* read ATmega port designated for 6502 data bus (DBUS) */
    cli();
    P6502_DBUS(DDR) = 0;
    P6502_DBUS(PORT) = 0;
    sei();
    return P6502_DBUS(PIN);
}
static inline void write_dbus(uint8_t v)
{
    /* write ATmega port designated for 6502 data bus (DBUS) */
    cli();
    P6502_DBUS(DDR) = 0xff;
    P6502_DBUS(PORT) = v;
    sei();
}

/* 6502 control */
static inline void setup_ictl()
{
    /* setup ATmega port designated for 6502 input control pins for output */
    cli();
    P6502_ICTL(DDR) = 0xff;
    sei();
}
static inline void setup_octl()
{
    /* setup ATmega port designated for 6502 output control pins for input */
    cli();
    P6502_OCTL(DDR) = 0;
    P6502_OCTL(PORT) = 0;
    sei();
}
static inline void write_ictl(uint8_t m, uint8_t v)
{
    /* write ATmega port designated for 6502 input control pins */
    cli();
    P6502_ICTL(PORT) = (P6502_ICTL(PORT) & ~m) | (v & m);
    sei();
}
static inline uint8_t read_octl()
{
    /* read ATmega port designated for 6502 output control pins */
    return P6502_OCTL(PIN);
}

/* clock */
#define CLK_DELAY()                     ((void)0)
//#define CLK_DELAY()                     delayMicroseconds(1)
static inline void clock_rise()
{
    write_ictl(P6502_ICTL_PIN(PHI2), 0xff);
    CLK_DELAY();
}
static inline void clock_fall()
{
    write_ictl(P6502_ICTL_PIN(PHI2), 0);
    CLK_DELAY();
}
static void clock_cycle(size_t n = 1)
{
    for (size_t i = 0; n > i; i++)
    {
        clock_rise();
        clock_fall();
    }
}

/* reset */
#define RESB_0_NCLK                     2
#define RESB_1_NCLK                     7
static void reset()
{
    write_ictl(P6502_ICTL_PIN(RESB), 0);
    clock_cycle(RESB_0_NCLK);
    write_ictl(P6502_ICTL_PIN(RESB), 0xff);
    clock_cycle(RESB_1_NCLK);
}

/* data read/write */
#define RAMSIZE                         (4 * 1024)
#define RAMMASK                         (RAMSIZE - 1)
#define ROMSIZE                         (16 * 1024)
#define ROMMASK                         (ROMSIZE - 1)
#define RAMADDR(a)                      ((a) < 0x8000)
#define MIOADDR(a)                      ((a) < 128)
static uint8_t ram[RAMSIZE];
static const uint8_t rom[ROMSIZE] PROGMEM =
{
    #include "6502rom.h"
};
static void write_mio(uint16_t addr, uint8_t data);
static inline uint8_t read_data(uint16_t addr)
{
    if (RAMADDR(addr))
        return ram[addr & RAMMASK];
    else
        return pgm_read_byte(&rom[addr & ROMMASK]);
}
static inline void write_data(uint16_t addr, uint8_t data)
{
    if (MIOADDR(addr))
        write_mio(addr, data);
    else
        ram[addr & RAMMASK] = data;
}

/* memory mapped I/O */
#define MIO_IRQN                        (0)
#define MIO_OREG                        (64)
#define MIO_OREGSIZE                    (32)
#define MIO_OREGMASK                    (MIO_OREGSIZE - 1)
#define MIO_IREG                        (96)
#define MIO_IREGSIZE                    (32)
#define MIO_IREGMASK                    (MIO_IREGSIZE - 1)
static void write_mio(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case MIO_IRQN:
        if (0 == data)
        {
            ram[addr] = data;
            write_ictl(P6502_ICTL_PIN(IRQB), 0xff);
        }
        break;
    case MIO_OREG:
        ram[addr] = data;
        if (0 != data)
        {
            Serial.write(ram + MIO_OREG + 1, data & MIO_OREGMASK);
            ram[MIO_OREG] = 0;
            ram[MIO_IRQN] = MIO_OREG;
            write_ictl(P6502_ICTL_PIN(IRQB), 0);
        }
        break;
    case MIO_IREG:
        ram[addr] = data;
        break;
    default:
        ram[addr] = data;
        break;
    }
}

/* debug */
static bool debug_step = 1;
size_t disasm(uint8_t (*read_data)(uint16_t), uint16_t addr, char buf[16]);
static void debug_header()
{
    Serial.begin(1000000);
    Serial.println();
    Serial.println("6502ctl:");
    Serial.println("    s to step");
    Serial.println("    c to continue");
    Serial.println("    b to break");
    Serial.println("    r to reset");
}
static void debug_prolog()
{
    while (debug_step || Serial.available())
    {
        int c;
        while (-1 == (c = Serial.read()))
            ;
        if (debug_step)
        {
            if ('s' == c)
                break;
            if ('c' == c)
            {
                debug_step = false;
                break;
            }
        }
        else
        {
            if ('b' == c)
                debug_step = true;
        }
        if ('r' == c)
            reset();
    }
}
static void debug_epilog(uint16_t addr, uint8_t data, uint8_t octl)
{
    if (debug_step)
    {
        char serbuf[64], disbuf[16];

        if (octl & P6502_OCTL_PIN(SYNC))
            disasm(read_data, addr, disbuf);

        snprintf(serbuf, sizeof serbuf, "%c%c%c%c %04x %02x%s%s",
            octl & P6502_OCTL_PIN(RWB) ? 'r' : 'W',
            octl & P6502_OCTL_PIN(SYNC) ? 'S' : '-',
            octl & P6502_OCTL_PIN(MLB) ? '-' : 'M',
            octl & P6502_OCTL_PIN(VPB) ? '-' : 'V',
            addr,
            data,
            octl & P6502_OCTL_PIN(SYNC) ? " " : "",
            octl & P6502_OCTL_PIN(SYNC) ? disbuf : "");
        Serial.println(serbuf);
    }
}

void setup()
{
    setup_abus();
    setup_dbus();
    setup_ictl();
    setup_octl();

    write_ictl(0xff,
        P6502_ICTL_PIN(RDY) |
        P6502_ICTL_PIN(IRQB) |
        P6502_ICTL_PIN(NMIB) |
        P6502_ICTL_PIN(SOB) |
        P6502_ICTL_PIN(BE));

    reset();

    debug_header();
}

void loop()
{
    uint16_t addr;
    uint8_t data;
    uint8_t octl;

    for (;;)
    {
        debug_prolog();

        clock_rise();

        octl = read_octl();
        addr = read_abus();
        if (octl & P6502_OCTL_PIN(RWB))
        {
            data = read_data(addr);
            write_dbus(data);
        }
        else
        {
            data = read_dbus();
            write_data(addr, data);
        }

        clock_fall();

        debug_epilog(addr, data, octl);
    }
}
