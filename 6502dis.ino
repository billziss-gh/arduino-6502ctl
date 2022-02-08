/*
 * 6502dis.ino
 * 6502 disassembler
 *
 * Copyright 2022 Bill Zissimopoulos
 */

struct ins
{
    enum
    {
        #define DISTAB_MNE_ENUM
        #include "6502distab.h"
        #undef DISTAB_MNE_ENUM
    };
    enum
    {
        MOD_INV = 0x01,                 /* invalid */
        MOD_ABS = 0x13,                 /* Absolute a */
        MOD_ABS_IND_X = 0x23,           /* Absolute Indexed Indirect (a,x) */
        MOD_ABS_X = 0x33,               /* Absolute Indexed with X a,x */
        MOD_ABS_Y = 0x43,               /* Absolute Indexed with Y a,y */
        MOD_ABS_IND = 0x53,             /* Absolute Indirect (a) */
        MOD_ACC = 0x61,                 /* Accumulator A */
        MOD_IMM = 0x72,                 /* Immediate Addressing # */
        MOD_IMP = 0x81,                 /* Implied i */
        MOD_PCR = 0x92,                 /* Program Counter Relative r */
        MOD_STK = 0xa1,                 /* Stack s */
        MOD_ZPG = 0xb2,                 /* Zero Page zp */
        MOD_ZPG_IND_X = 0xc2,           /* Zero Page Indexed Indirect (zp,x) */
        MOD_ZPG_X = 0xd2,               /* Zero Page Indexed with X zp,x */
        MOD_ZPG_Y = 0xe2,               /* Zero Page Indexed with Y zp,y */
        MOD_ZPG_IND = 0xf2,             /* Zero Page Indirect (zp) */
        MOD_ZPG_IND_Y = 0x02,           /* Zero Page Indirect Indexed with Y (zp),y */
    };
    uint8_t mne;
    uint8_t mod;
    uint8_t len() const
    {
        return mod & 0xf;
    }
};

static const ins ins_tab[256] PROGMEM =
{
    #define DISTAB_INS
    #include "6502distab.h"
    #undef DISTAB_INS
};
static const char mne_tab[][5] PROGMEM =
{
    #define DISTAB_MNE_STR
    #include "6502distab.h"
    #undef DISTAB_MNE_STR
};

static size_t dishex(char *buf, uint16_t val, size_t len)
{
    size_t len0 = len;
    while (0 != len)
    {
        uint8_t c = val & 0xf;
        val >>= 4;
        buf[--len] = 10 > c ? c + '0' : c - 10 + 'a';
    }
    return len0;
}

struct ins decode(uint8_t opc)
{
    const ins *i = &ins_tab[opc];
    return ins{ pgm_read_byte(&i->mne), pgm_read_byte(&i->mod) };
}

size_t disasm(uint8_t (*read_data)(uint16_t), uint16_t addr, char buf[16])
{
    uint8_t opc = read_data(addr + 0);
    uint16_t val = 0;
    char *p;
    ins i = decode(opc);

    p = buf;
    for (size_t j = 0;; j++)
    {
        uint8_t c = pgm_read_byte(&mne_tab[i.mne][j]);
        if ('\0' == c)
            break;
        *p++ = c;
    }

    switch (i.len())
    {
    case 2:
        val = read_data(addr + 1);
        break;
    case 3:
        val = read_data(addr + 1);
        val |= read_data(addr + 2) << 8;
        break;
    }

    switch (i.mod)
    {
    case ins::MOD_INV:
        break;
    case ins::MOD_ABS:
        *p++ = ' ';
        *p++ = '$';
        p += dishex(p, val, 4);
        break;
    case ins::MOD_ABS_IND_X:
        *p++ = ' ';
        *p++ = '(';
        *p++ = '$';
        p += dishex(p, val, 4);
        *p++ = ',';
        *p++ = 'X';
        *p++ = ')';
        break;
    case ins::MOD_ABS_X:
        *p++ = ' ';
        *p++ = '$';
        p += dishex(p, val, 4);
        *p++ = ',';
        *p++ = 'X';
        break;
    case ins::MOD_ABS_Y:
        *p++ = ' ';
        *p++ = '$';
        p += dishex(p, val, 4);
        *p++ = ',';
        *p++ = 'Y';
        break;
    case ins::MOD_ABS_IND:
        *p++ = ' ';
        *p++ = '(';
        *p++ = '$';
        p += dishex(p, val, 4);
        *p++ = ')';
        break;
    case ins::MOD_ACC:
        break;
    case ins::MOD_IMM:
        *p++ = ' ';
        *p++ = '#';
        *p++ = '$';
        p += dishex(p, val, 2);
        break;
    case ins::MOD_IMP:
        break;
    case ins::MOD_PCR:
        *p++ = ' ';
        *p++ = '#';
        *p++ = '$';
        p += dishex(p, val, 2);
        break;
    case ins::MOD_STK:
        break;
    case ins::MOD_ZPG:
        *p++ = ' ';
        *p++ = '$';
        p += dishex(p, val, 2);
        break;
    case ins::MOD_ZPG_IND_X:
        *p++ = ' ';
        *p++ = '(';
        *p++ = '$';
        p += dishex(p, val, 2);
        *p++ = ',';
        *p++ = 'X';
        *p++ = ')';
        break;
    case ins::MOD_ZPG_X:
        *p++ = ' ';
        *p++ = '$';
        p += dishex(p, val, 2);
        *p++ = ',';
        *p++ = 'X';
        break;
    case ins::MOD_ZPG_Y:
        *p++ = ' ';
        *p++ = '$';
        p += dishex(p, val, 2);
        *p++ = ',';
        *p++ = 'Y';
        break;
    case ins::MOD_ZPG_IND:
        *p++ = ' ';
        *p++ = '(';
        *p++ = '$';
        p += dishex(p, val, 2);
        *p++ = ')';
        break;
    case ins::MOD_ZPG_IND_Y:
        *p++ = ' ';
        *p++ = '(';
        *p++ = '$';
        p += dishex(p, val, 4);
        *p++ = ')';
        *p++ = ',';
        *p++ = 'Y';
        break;
    }

    *p++ = '\0';

    return p - buf;
}
