/*
 * 6502ctl.ino
 * 6502 controller
 *
 * Copyright 2022 Bill Zissimopoulos
 */

#define SERIAL_SPEED                    1000000

/*
 * W65C02S
 * https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf
 */

/* 6502 address bus pins (PF0-7, PK0-7) */
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

/* 6502 data bus pins (PL0-7) */
#define PIN6502_D0                      49
#define PIN6502_D1                      48
#define PIN6502_D2                      47
#define PIN6502_D3                      46
#define PIN6502_D4                      45
#define PIN6502_D5                      44
#define PIN6502_D6                      43
#define PIN6502_D7                      42

/* 6502 control pins */
#define PIN6502_VPB                     22
#define PIN6502_RDY                     24
#define PIN6502_PHI1O                   26
#define PIN6502_IRQB                    28
#define PIN6502_MLB                     30
#define PIN6502_NMIB                    32
#define PIN6502_SYNC                    34
#define PIN6502_RESB                    23
#define PIN6502_PHI2O                   25
#define PIN6502_SOB                     27
#define PIN6502_PHI2                    29
#define PIN6502_BE                      31
#define PIN6502_RWB                     33

/* 6502 address bus */
static void setup_abus()
{
    /* set Arduino pins 54-69 (ATmega 2560 PF, PK) to INPUT */
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
static void setup_dbus()
{
    /* set Arduino pins 42-49 (ATmega 2560 PL) to INPUT */
    cli();
    DDRL = 0;
    PORTL = 0;
    sei();
}
static inline uint8_t read_dbus()
{
    /* set PL to INPUT and read */
    cli();
    DDRL = 0;
    PORTL = 0;
    sei();
    return PINL;
}
static inline void write_dbus(uint8_t v)
{
    /* set PL to OUTPUT and write */
    cli();
    DDRL = 0xff;
    PORTL = v;
    sei();
}

/* 6502 control */
static const uint8_t pin_icontrol[] =
{
    PIN6502_RDY,
    PIN6502_IRQB,
    PIN6502_NMIB,
    PIN6502_PHI2,
    PIN6502_SOB,
    PIN6502_RESB,
    PIN6502_BE,
};
static const uint8_t pin_ocontrol[] =
{
    PIN6502_VPB,
    PIN6502_PHI1O,
    PIN6502_MLB,
    PIN6502_SYNC,
    PIN6502_RWB,
    PIN6502_PHI2O,
};
static void setup_control()
{
    for (size_t i = 0; sizeof pin_icontrol / sizeof pin_icontrol[0] > i; i++)
        pinMode(pin_icontrol[i], OUTPUT);
    for (size_t i = 0; sizeof pin_ocontrol / sizeof pin_ocontrol[0] > i; i++)
        pinMode(pin_ocontrol[i], INPUT);
}

/* clock */
#define CLK_DELAY()                     delayMicroseconds(1)
static void clock_rise()
{
    digitalWrite(PIN6502_PHI2, 1);
    CLK_DELAY();
}
static void clock_fall()
{
    digitalWrite(PIN6502_PHI2, 0);
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
