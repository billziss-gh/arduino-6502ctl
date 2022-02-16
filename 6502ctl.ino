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
    /* set Arduino 54-69 (ATmega2560 PF, PK) to input */
    cli();
    DDRF = 0;
    PORTF = 0;
    DDRK = 0;
    PORTK = 0;
    sei();
}
static inline uint16_t read_abus()
{
    return PINF | (PINK << 8);
}

/* 6502 data bus */
static inline void setup_dbus()
{
    /* set Arduino 42-49 (ATmega2560 PL) to input */
    cli();
    DDRL = 0;
    PORTL = 0;
    sei();
}
static inline uint8_t read_dbus()
{
    /* set ATmega2560 PL to input and read */
    cli();
    DDRL = 0;
    PORTL = 0;
    sei();
    return PINL;
}
static inline void write_dbus(uint8_t v)
{
    /* set ATmega2560 PL to output and write */
    cli();
    DDRL = 0xff;
    PORTL = v;
    sei();
}

/* 6502 control */
static inline void setup_control()
{
    /* set Arduino pins 22-28 (ATmega2560 PA) to output and pins 20-35 (ATmega2560 PC) to input */
    cli();
    DDRA = 0xff;
    DDRC = 0;
    PORTC = 0;
    sei();
}

/* clock */
#define CLK_DELAY()                     delayMicroseconds(1)
static inline void clock_rise()
{
    digitalWrite(PIN6502_PHI2, 1);
    CLK_DELAY();
}
static inline void clock_fall()
{
    digitalWrite(PIN6502_PHI2, 0);
    CLK_DELAY();
}
static inline void clock_cycle(size_t n = 1)
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
    digitalWrite(PIN6502_RESB, 0);
    clock_cycle(RESB_0_NCLK);
    digitalWrite(PIN6502_RESB, 1);
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
            digitalWrite(PIN6502_IRQB, 1);
        }
        break;
    case MIO_OREG:
        mio[addr] = data;
        if (0 != data)
        {
            Serial.write(mio + MIO_OBUF + 1, data & MIO_OBUFMASK);
            mio[MIO_OREG] = 0;
            mio[MIO_IRQN] = MIO_OREG;
            digitalWrite(PIN6502_IRQB, 0);
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
    setup_control();

    digitalWrite(PIN6502_RDY, 1);
    digitalWrite(PIN6502_IRQB, 1);
    digitalWrite(PIN6502_NMIB, 1);
    digitalWrite(PIN6502_SOB, 1);
    digitalWrite(PIN6502_BE, 1);

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
    uint8_t rwb, sync, mlb, vpb;
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

    addr = read_abus();
    rwb = digitalRead(PIN6502_RWB);
    if (rwb)
    {
        data = read_data(addr);
        write_dbus(data);
    }
    else
    {
        data = read_dbus();
        write_data(addr, data);
    }

    sync = digitalRead(PIN6502_SYNC);
    mlb = digitalRead(PIN6502_MLB);
    vpb = digitalRead(PIN6502_VPB);

    clock_fall();

    if (step)
    {
        if (sync)
            disasm(read_data, addr, disbuf);

        snprintf(serbuf, sizeof serbuf, "%c%c%c%c %04x %02x%s%s",
            rwb ? 'r' : 'W',
            sync ? 'S' : '-',
            mlb ? '-' : 'M',
            vpb ? '-' : 'V',
            addr,
            data,
            sync ? " " : "",
            sync ? disbuf : "");
        Serial.println(serbuf);
    }
}
