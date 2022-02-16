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
#define RAMRGN0                         (0)
#define RAMRGN1                         (0x6fff)
#define MIOSIZE                         (128)
#define MIOMASK                         (MIOSIZE - 1)
#define MIORGN0                         (0x7000)
#define MIORGN1                         (0x7fff)
#define ROMSIZE                         (16 * 1024)
#define ROMMASK                         (ROMSIZE - 1)
#define ROMRGN0                         (0x8000)
#define ROMRGN1                         (0xffff)
static uint8_t ram[RAMSIZE];
static uint8_t mio[MIOSIZE];
static const uint8_t rom[ROMSIZE] PROGMEM =
{
    #include "6502rom.h"
};
static uint8_t read_mio(uint16_t addr);
static void write_mio(uint16_t addr, uint8_t data);
static uint8_t read_data(uint16_t addr)
{
    uint8_t data;
    if (RAMRGN1 >= addr)
        data = ram[addr & RAMMASK];
    else if (MIORGN1 >= addr)
        data = read_mio(addr & MIOMASK);
    else
        data = pgm_read_byte(&rom[addr & ROMMASK]);
    return data;
}
static void write_data(uint16_t addr, uint8_t data)
{
    if (RAMRGN1 >= addr)
        ram[addr & RAMMASK] = data;
    else if (MIORGN1 >= addr)
        write_mio(addr & MIOMASK, data);
}

/* memory mapped I/O */
#define MIO_IRQN                        0
#define MIO_OREG                        1
#define MIO_IREG                        2
#define MIO_OBUF                        32
#define MIO_OBUFSIZE                    32
#define MIO_OBUFMASK                    (MIO_OBUFSIZE - 1)
#define MIO_IBUF                        64
#define MIO_IBUFSIZE                    32
#define MIO_IBUFMASK                    (MIO_IBUFSIZE - 1)
static uint8_t read_mio(uint16_t addr)
{
    return mio[addr];
}
static void write_mio(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case MIO_IRQN:
        if (0 == data)
        {
            mio[addr] = data;
            write_ictl(P6502_ICTL_PIN(IRQB), 0xff);
        }
        break;
    case MIO_OREG:
        mio[addr] = data;
        if (0 != data)
        {
            Serial.write(mio + MIO_OBUF + 1, data & MIO_OBUFMASK);
            mio[MIO_OREG] = 0;
            mio[MIO_IRQN] = MIO_OREG;
            write_ictl(P6502_ICTL_PIN(IRQB), 0);
        }
        break;
    case MIO_IREG:
        mio[addr] = data;
        break;
    default:
        mio[addr] = data;
        break;
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

    Serial.begin(1000000);
    Serial.println();
    Serial.println("6502ctl:");
    Serial.println("    s to step");
    Serial.println("    c to continue");
    Serial.println("    b to break");
    Serial.println("    r to reset");
}

size_t disasm(uint8_t (*read_data)(uint16_t), uint16_t addr, char buf[16]);
void loop()
{
    static bool step = 1;
    uint16_t addr;
    uint8_t data;
    uint8_t octl;
    char serbuf[64], disbuf[16];
    int c;

    while (step || Serial.available())
    {
        while (-1 == (c = Serial.read()))
            ;
        if (step)
        {
            if ('s' == c)
                break;
            if ('c' == c)
            {
                step = false;
                break;
            }
        }
        else
        {
            if ('b' == c)
                step = true;
        }
        if ('r' == c)
            reset();
    }

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

    if (step)
    {
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
