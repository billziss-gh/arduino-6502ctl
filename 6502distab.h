#if defined(DISTAB_MNE_ENUM)
    MNE_ADC = 0,
    MNE_AND = 1,
    MNE_ASL = 2,
    MNE_BBR0 = 3,
    MNE_BBR1 = 4,
    MNE_BBR2 = 5,
    MNE_BBR3 = 6,
    MNE_BBR4 = 7,
    MNE_BBR5 = 8,
    MNE_BBR6 = 9,
    MNE_BBR7 = 10,
    MNE_BBS0 = 11,
    MNE_BBS1 = 12,
    MNE_BBS2 = 13,
    MNE_BBS3 = 14,
    MNE_BBS4 = 15,
    MNE_BBS5 = 16,
    MNE_BBS6 = 17,
    MNE_BBS7 = 18,
    MNE_BCC = 19,
    MNE_BCS = 20,
    MNE_BEQ = 21,
    MNE_BIT = 22,
    MNE_BMI = 23,
    MNE_BNE = 24,
    MNE_BPL = 25,
    MNE_BRA = 26,
    MNE_BRK = 27,
    MNE_BVC = 28,
    MNE_BVS = 29,
    MNE_CLC = 30,
    MNE_CLD = 31,
    MNE_CLI = 32,
    MNE_CLV = 33,
    MNE_CMP = 34,
    MNE_CPX = 35,
    MNE_CPY = 36,
    MNE_DEC = 37,
    MNE_DEX = 38,
    MNE_DEY = 39,
    MNE_EOR = 40,
    MNE_INC = 41,
    MNE_INVL = 42,
    MNE_INX = 43,
    MNE_INY = 44,
    MNE_JMP = 45,
    MNE_JSR = 46,
    MNE_LDA = 47,
    MNE_LDX = 48,
    MNE_LDY = 49,
    MNE_LSR = 50,
    MNE_NOP = 51,
    MNE_ORA = 52,
    MNE_PHA = 53,
    MNE_PHP = 54,
    MNE_PHX = 55,
    MNE_PHY = 56,
    MNE_PLA = 57,
    MNE_PLP = 58,
    MNE_PLX = 59,
    MNE_PLY = 60,
    MNE_RMB0 = 61,
    MNE_RMB1 = 62,
    MNE_RMB2 = 63,
    MNE_RMB3 = 64,
    MNE_RMB4 = 65,
    MNE_RMB5 = 66,
    MNE_RMB6 = 67,
    MNE_RMB7 = 68,
    MNE_ROL = 69,
    MNE_ROR = 70,
    MNE_RTI = 71,
    MNE_RTS = 72,
    MNE_SBC = 73,
    MNE_SEC = 74,
    MNE_SED = 75,
    MNE_SEI = 76,
    MNE_SMB0 = 77,
    MNE_SMB1 = 78,
    MNE_SMB2 = 79,
    MNE_SMB3 = 80,
    MNE_SMB4 = 81,
    MNE_SMB5 = 82,
    MNE_SMB6 = 83,
    MNE_SMB7 = 84,
    MNE_STA = 85,
    MNE_STP = 86,
    MNE_STX = 87,
    MNE_STY = 88,
    MNE_STZ = 89,
    MNE_TAX = 90,
    MNE_TAY = 91,
    MNE_TRB = 92,
    MNE_TSB = 93,
    MNE_TSX = 94,
    MNE_TXA = 95,
    MNE_TXS = 96,
    MNE_TYA = 97,
    MNE_WAI = 98,
#elif defined(DISTAB_MNE_STR)
    "ADC",
    "AND",
    "ASL",
    "BBR0",
    "BBR1",
    "BBR2",
    "BBR3",
    "BBR4",
    "BBR5",
    "BBR6",
    "BBR7",
    "BBS0",
    "BBS1",
    "BBS2",
    "BBS3",
    "BBS4",
    "BBS5",
    "BBS6",
    "BBS7",
    "BCC",
    "BCS",
    "BEQ",
    "BIT",
    "BMI",
    "BNE",
    "BPL",
    "BRA",
    "BRK",
    "BVC",
    "BVS",
    "CLC",
    "CLD",
    "CLI",
    "CLV",
    "CMP",
    "CPX",
    "CPY",
    "DEC",
    "DEX",
    "DEY",
    "EOR",
    "INC",
    "INVL",
    "INX",
    "INY",
    "JMP",
    "JSR",
    "LDA",
    "LDX",
    "LDY",
    "LSR",
    "NOP",
    "ORA",
    "PHA",
    "PHP",
    "PHX",
    "PHY",
    "PLA",
    "PLP",
    "PLX",
    "PLY",
    "RMB0",
    "RMB1",
    "RMB2",
    "RMB3",
    "RMB4",
    "RMB5",
    "RMB6",
    "RMB7",
    "ROL",
    "ROR",
    "RTI",
    "RTS",
    "SBC",
    "SEC",
    "SED",
    "SEI",
    "SMB0",
    "SMB1",
    "SMB2",
    "SMB3",
    "SMB4",
    "SMB5",
    "SMB6",
    "SMB7",
    "STA",
    "STP",
    "STX",
    "STY",
    "STZ",
    "TAX",
    "TAY",
    "TRB",
    "TSB",
    "TSX",
    "TXA",
    "TXS",
    "TYA",
    "WAI",
#elif defined(DISTAB_INS)
    [0x00] = { ins::MNE_BRK, ins::MOD_IMM },                    /* BRK # */
    [0x01] = { ins::MNE_ORA, ins::MOD_ZPG_IND_X },              /* ORA (zp,x) */
    [0x02] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x03] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x04] = { ins::MNE_TSB, ins::MOD_ZPG },                    /* TSB zp */
    [0x05] = { ins::MNE_ORA, ins::MOD_ZPG },                    /* ORA zp */
    [0x06] = { ins::MNE_ASL, ins::MOD_ZPG },                    /* ASL zp */
    [0x07] = { ins::MNE_RMB0, ins::MOD_ZPG },                   /* RMB0 zp */
    [0x08] = { ins::MNE_PHP, ins::MOD_STK },                    /* PHP s */
    [0x09] = { ins::MNE_ORA, ins::MOD_IMM },                    /* ORA # */
    [0x0a] = { ins::MNE_ASL, ins::MOD_ACC },                    /* ASL A */
    [0x0b] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x0c] = { ins::MNE_TSB, ins::MOD_ABS },                    /* TSB a */
    [0x0d] = { ins::MNE_ORA, ins::MOD_ABS },                    /* ORA a */
    [0x0e] = { ins::MNE_ASL, ins::MOD_ABS },                    /* ASL a */
    [0x0f] = { ins::MNE_BBR0, ins::MOD_PCR },                   /* BBR0 r */
    [0x10] = { ins::MNE_BPL, ins::MOD_PCR },                    /* BPL r */
    [0x11] = { ins::MNE_ORA, ins::MOD_ZPG_IND_Y },              /* ORA (zp),y */
    [0x12] = { ins::MNE_ORA, ins::MOD_ZPG_IND },                /* ORA (zp) */
    [0x13] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x14] = { ins::MNE_TRB, ins::MOD_ZPG },                    /* TRB zp */
    [0x15] = { ins::MNE_ORA, ins::MOD_ZPG_X },                  /* ORA zp,x */
    [0x16] = { ins::MNE_ASL, ins::MOD_ZPG_X },                  /* ASL zp,x */
    [0x17] = { ins::MNE_RMB1, ins::MOD_ZPG },                   /* RMB1 zp */
    [0x18] = { ins::MNE_CLC, ins::MOD_IMP },                    /* CLC i */
    [0x19] = { ins::MNE_ORA, ins::MOD_ABS_Y },                  /* ORA a,y */
    [0x1a] = { ins::MNE_INC, ins::MOD_ACC },                    /* INC A */
    [0x1b] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x1c] = { ins::MNE_TRB, ins::MOD_ABS },                    /* TRB a */
    [0x1d] = { ins::MNE_ORA, ins::MOD_ABS_X },                  /* ORA a,x */
    [0x1e] = { ins::MNE_ASL, ins::MOD_ABS_X },                  /* ASL a,x */
    [0x1f] = { ins::MNE_BBR1, ins::MOD_PCR },                   /* BBR1 r */
    [0x20] = { ins::MNE_JSR, ins::MOD_ABS },                    /* JSR a */
    [0x21] = { ins::MNE_AND, ins::MOD_ZPG_IND_X },              /* AND (zp,x) */
    [0x22] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x23] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x24] = { ins::MNE_BIT, ins::MOD_ZPG },                    /* BIT zp */
    [0x25] = { ins::MNE_AND, ins::MOD_ZPG },                    /* AND zp */
    [0x26] = { ins::MNE_ROL, ins::MOD_ZPG },                    /* ROL zp */
    [0x27] = { ins::MNE_RMB2, ins::MOD_ZPG },                   /* RMB2 zp */
    [0x28] = { ins::MNE_PLP, ins::MOD_STK },                    /* PLP s */
    [0x29] = { ins::MNE_AND, ins::MOD_IMM },                    /* AND # */
    [0x2a] = { ins::MNE_ROL, ins::MOD_ACC },                    /* ROL A */
    [0x2b] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x2c] = { ins::MNE_BIT, ins::MOD_ABS },                    /* BIT a */
    [0x2d] = { ins::MNE_AND, ins::MOD_ABS },                    /* AND a */
    [0x2e] = { ins::MNE_ROL, ins::MOD_ABS },                    /* ROL a */
    [0x2f] = { ins::MNE_BBR2, ins::MOD_PCR },                   /* BBR2 r */
    [0x30] = { ins::MNE_BMI, ins::MOD_PCR },                    /* BMI r */
    [0x31] = { ins::MNE_AND, ins::MOD_ZPG_IND_Y },              /* AND (zp),y */
    [0x32] = { ins::MNE_AND, ins::MOD_ZPG_IND },                /* AND (zp) */
    [0x33] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x34] = { ins::MNE_BIT, ins::MOD_ZPG_X },                  /* BIT zp,x */
    [0x35] = { ins::MNE_AND, ins::MOD_ZPG_X },                  /* AND zp,x */
    [0x36] = { ins::MNE_ROL, ins::MOD_ZPG_X },                  /* ROL zp,x */
    [0x37] = { ins::MNE_RMB3, ins::MOD_ZPG },                   /* RMB3 zp */
    [0x38] = { ins::MNE_SEC, ins::MOD_IMP },                    /* SEC i */
    [0x39] = { ins::MNE_AND, ins::MOD_ABS_Y },                  /* AND a,y */
    [0x3a] = { ins::MNE_DEC, ins::MOD_ACC },                    /* DEC A */
    [0x3b] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x3c] = { ins::MNE_BIT, ins::MOD_ABS_X },                  /* BIT a,x */
    [0x3d] = { ins::MNE_AND, ins::MOD_ABS_X },                  /* AND a,x */
    [0x3e] = { ins::MNE_ROL, ins::MOD_ABS_X },                  /* ROL a,x */
    [0x3f] = { ins::MNE_BBR3, ins::MOD_PCR },                   /* BBR3 r */
    [0x40] = { ins::MNE_RTI, ins::MOD_STK },                    /* RTI s */
    [0x41] = { ins::MNE_EOR, ins::MOD_ZPG_IND_X },              /* EOR (zp,x) */
    [0x42] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x43] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x44] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x45] = { ins::MNE_EOR, ins::MOD_ZPG },                    /* EOR zp */
    [0x46] = { ins::MNE_LSR, ins::MOD_ZPG },                    /* LSR zp */
    [0x47] = { ins::MNE_RMB4, ins::MOD_ZPG },                   /* RMB4 zp */
    [0x48] = { ins::MNE_PHA, ins::MOD_STK },                    /* PHA s */
    [0x49] = { ins::MNE_EOR, ins::MOD_IMM },                    /* EOR # */
    [0x4a] = { ins::MNE_LSR, ins::MOD_ACC },                    /* LSR A */
    [0x4b] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x4c] = { ins::MNE_JMP, ins::MOD_ABS },                    /* JMP a */
    [0x4d] = { ins::MNE_EOR, ins::MOD_ABS },                    /* EOR a */
    [0x4e] = { ins::MNE_LSR, ins::MOD_ABS },                    /* LSR a */
    [0x4f] = { ins::MNE_BBR4, ins::MOD_PCR },                   /* BBR4 r */
    [0x50] = { ins::MNE_BVC, ins::MOD_PCR },                    /* BVC r */
    [0x51] = { ins::MNE_EOR, ins::MOD_ZPG_IND_Y },              /* EOR (zp),y */
    [0x52] = { ins::MNE_EOR, ins::MOD_ZPG_IND },                /* EOR (zp) */
    [0x53] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x54] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x55] = { ins::MNE_EOR, ins::MOD_ZPG_X },                  /* EOR zp,x */
    [0x56] = { ins::MNE_LSR, ins::MOD_ZPG_X },                  /* LSR zp,x */
    [0x57] = { ins::MNE_RMB5, ins::MOD_ZPG },                   /* RMB5 zp */
    [0x58] = { ins::MNE_CLI, ins::MOD_IMP },                    /* CLI i */
    [0x59] = { ins::MNE_EOR, ins::MOD_ABS_Y },                  /* EOR a,y */
    [0x5a] = { ins::MNE_PHY, ins::MOD_STK },                    /* PHY s */
    [0x5b] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x5c] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x5d] = { ins::MNE_EOR, ins::MOD_ABS_X },                  /* EOR a,x */
    [0x5e] = { ins::MNE_LSR, ins::MOD_ABS_X },                  /* LSR a,x */
    [0x5f] = { ins::MNE_BBR5, ins::MOD_PCR },                   /* BBR5 r */
    [0x60] = { ins::MNE_RTS, ins::MOD_STK },                    /* RTS s */
    [0x61] = { ins::MNE_ADC, ins::MOD_ZPG_IND_X },              /* ADC (zp,x) */
    [0x62] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x63] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x64] = { ins::MNE_STZ, ins::MOD_ZPG },                    /* STZ zp */
    [0x65] = { ins::MNE_ADC, ins::MOD_ZPG },                    /* ADC zp */
    [0x66] = { ins::MNE_ROR, ins::MOD_ZPG },                    /* ROR zp */
    [0x67] = { ins::MNE_RMB6, ins::MOD_ZPG },                   /* RMB6 zp */
    [0x68] = { ins::MNE_PLA, ins::MOD_STK },                    /* PLA s */
    [0x69] = { ins::MNE_ADC, ins::MOD_IMM },                    /* ADC # */
    [0x6a] = { ins::MNE_ROR, ins::MOD_ACC },                    /* ROR A */
    [0x6b] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x6c] = { ins::MNE_JMP, ins::MOD_ABS_IND },                /* JMP (a) */
    [0x6d] = { ins::MNE_ADC, ins::MOD_ABS },                    /* ADC a */
    [0x6e] = { ins::MNE_ROR, ins::MOD_ABS },                    /* ROR a */
    [0x6f] = { ins::MNE_BBR6, ins::MOD_PCR },                   /* BBR6 r */
    [0x70] = { ins::MNE_BVS, ins::MOD_PCR },                    /* BVS r */
    [0x71] = { ins::MNE_ADC, ins::MOD_ZPG_IND_Y },              /* ADC (zp),y */
    [0x72] = { ins::MNE_ADC, ins::MOD_ZPG_IND },                /* ADC (zp) */
    [0x73] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x74] = { ins::MNE_STZ, ins::MOD_ZPG_X },                  /* STZ zp,x */
    [0x75] = { ins::MNE_ADC, ins::MOD_ZPG_X },                  /* ADC zp,x */
    [0x76] = { ins::MNE_ROR, ins::MOD_ZPG_X },                  /* ROR zp,x */
    [0x77] = { ins::MNE_RMB7, ins::MOD_ZPG },                   /* RMB7 zp */
    [0x78] = { ins::MNE_SEI, ins::MOD_IMP },                    /* SEI i */
    [0x79] = { ins::MNE_ADC, ins::MOD_ABS_Y },                  /* ADC a,y */
    [0x7a] = { ins::MNE_PLY, ins::MOD_STK },                    /* PLY s */
    [0x7b] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x7c] = { ins::MNE_JMP, ins::MOD_ABS_IND_X },              /* JMP (a,x) */
    [0x7d] = { ins::MNE_ADC, ins::MOD_ABS_X },                  /* ADC a,x */
    [0x7e] = { ins::MNE_ROR, ins::MOD_ABS_X },                  /* ROR a,x */
    [0x7f] = { ins::MNE_BBR7, ins::MOD_PCR },                   /* BBR7 r */
    [0x80] = { ins::MNE_BRA, ins::MOD_PCR },                    /* BRA r */
    [0x81] = { ins::MNE_STA, ins::MOD_ZPG_IND_X },              /* STA (zp,x) */
    [0x82] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x83] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x84] = { ins::MNE_STY, ins::MOD_ZPG },                    /* STY zp */
    [0x85] = { ins::MNE_STA, ins::MOD_ZPG },                    /* STA zp */
    [0x86] = { ins::MNE_STX, ins::MOD_ZPG },                    /* STX zp */
    [0x87] = { ins::MNE_SMB0, ins::MOD_ZPG },                   /* SMB0 zp */
    [0x88] = { ins::MNE_DEY, ins::MOD_IMP },                    /* DEY i */
    [0x89] = { ins::MNE_BIT, ins::MOD_IMM },                    /* BIT # */
    [0x8a] = { ins::MNE_TXA, ins::MOD_IMP },                    /* TXA i */
    [0x8b] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x8c] = { ins::MNE_STY, ins::MOD_ABS },                    /* STY a */
    [0x8d] = { ins::MNE_STA, ins::MOD_ABS },                    /* STA a */
    [0x8e] = { ins::MNE_STX, ins::MOD_ABS },                    /* STX a */
    [0x8f] = { ins::MNE_BBS0, ins::MOD_PCR },                   /* BBS0 r */
    [0x90] = { ins::MNE_BCC, ins::MOD_PCR },                    /* BCC r */
    [0x91] = { ins::MNE_STA, ins::MOD_ZPG_IND_Y },              /* STA (zp),y */
    [0x92] = { ins::MNE_STA, ins::MOD_ZPG_IND },                /* STA (zp) */
    [0x93] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x94] = { ins::MNE_STY, ins::MOD_ZPG_X },                  /* STY zp,x */
    [0x95] = { ins::MNE_STA, ins::MOD_ZPG_X },                  /* STA zp,x */
    [0x96] = { ins::MNE_STX, ins::MOD_ZPG_Y },                  /* STX zp,y */
    [0x97] = { ins::MNE_SMB1, ins::MOD_ZPG },                   /* SMB1 zp */
    [0x98] = { ins::MNE_TYA, ins::MOD_IMP },                    /* TYA i */
    [0x99] = { ins::MNE_STA, ins::MOD_ABS_Y },                  /* STA a,y */
    [0x9a] = { ins::MNE_TXS, ins::MOD_IMP },                    /* TXS i */
    [0x9b] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0x9c] = { ins::MNE_STZ, ins::MOD_ABS },                    /* STZ a */
    [0x9d] = { ins::MNE_STA, ins::MOD_ABS_X },                  /* STA a,x */
    [0x9e] = { ins::MNE_STZ, ins::MOD_ABS_X },                  /* STZ a,x */
    [0x9f] = { ins::MNE_BBS1, ins::MOD_PCR },                   /* BBS1 r */
    [0xa0] = { ins::MNE_LDY, ins::MOD_IMM },                    /* LDY # */
    [0xa1] = { ins::MNE_LDA, ins::MOD_ZPG_IND_X },              /* LDA (zp,x) */
    [0xa2] = { ins::MNE_LDX, ins::MOD_IMM },                    /* LDX # */
    [0xa3] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xa4] = { ins::MNE_LDY, ins::MOD_ZPG },                    /* LDY zp */
    [0xa5] = { ins::MNE_LDA, ins::MOD_ZPG },                    /* LDA zp */
    [0xa6] = { ins::MNE_LDX, ins::MOD_ZPG },                    /* LDX zp */
    [0xa7] = { ins::MNE_SMB2, ins::MOD_ZPG },                   /* SMB2 zp */
    [0xa8] = { ins::MNE_TAY, ins::MOD_IMP },                    /* TAY i */
    [0xa9] = { ins::MNE_LDA, ins::MOD_IMM },                    /* LDA # */
    [0xaa] = { ins::MNE_TAX, ins::MOD_IMP },                    /* TAX i */
    [0xab] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xac] = { ins::MNE_LDY, ins::MOD_ABS },                    /* LDY a */
    [0xad] = { ins::MNE_LDA, ins::MOD_ABS },                    /* LDA a */
    [0xae] = { ins::MNE_LDX, ins::MOD_ABS },                    /* LDX a */
    [0xaf] = { ins::MNE_BBS2, ins::MOD_PCR },                   /* BBS2 r */
    [0xb0] = { ins::MNE_BCS, ins::MOD_PCR },                    /* BCS r */
    [0xb1] = { ins::MNE_LDA, ins::MOD_ZPG_IND_Y },              /* LDA (zp),y */
    [0xb2] = { ins::MNE_LDA, ins::MOD_ZPG_IND },                /* LDA (zp) */
    [0xb3] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xb4] = { ins::MNE_LDY, ins::MOD_ZPG_X },                  /* LDY zp,x */
    [0xb5] = { ins::MNE_LDA, ins::MOD_ZPG_X },                  /* LDA zp,x */
    [0xb6] = { ins::MNE_LDX, ins::MOD_ZPG_Y },                  /* LDX zp,y */
    [0xb7] = { ins::MNE_SMB3, ins::MOD_ZPG },                   /* SMB3 zp */
    [0xb8] = { ins::MNE_CLV, ins::MOD_IMP },                    /* CLV i */
    [0xb9] = { ins::MNE_LDA, ins::MOD_ABS_Y },                  /* LDA a,y */
    [0xba] = { ins::MNE_TSX, ins::MOD_IMP },                    /* TSX i */
    [0xbb] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xbc] = { ins::MNE_LDY, ins::MOD_ABS_X },                  /* LDY a,x */
    [0xbd] = { ins::MNE_LDA, ins::MOD_ABS_X },                  /* LDA a,x */
    [0xbe] = { ins::MNE_LDX, ins::MOD_ABS_Y },                  /* LDX a,y */
    [0xbf] = { ins::MNE_BBS3, ins::MOD_PCR },                   /* BBS3 r */
    [0xc0] = { ins::MNE_CPY, ins::MOD_IMM },                    /* CPY # */
    [0xc1] = { ins::MNE_CMP, ins::MOD_ZPG_IND_X },              /* CMP (zp,x) */
    [0xc2] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xc3] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xc4] = { ins::MNE_CPY, ins::MOD_ZPG },                    /* CPY zp */
    [0xc5] = { ins::MNE_CMP, ins::MOD_ZPG },                    /* CMP zp */
    [0xc6] = { ins::MNE_DEC, ins::MOD_ZPG },                    /* DEC zp */
    [0xc7] = { ins::MNE_SMB4, ins::MOD_ZPG },                   /* SMB4 zp */
    [0xc8] = { ins::MNE_INY, ins::MOD_IMP },                    /* INY i */
    [0xc9] = { ins::MNE_CMP, ins::MOD_IMM },                    /* CMP # */
    [0xca] = { ins::MNE_DEX, ins::MOD_IMP },                    /* DEX i */
    [0xcb] = { ins::MNE_WAI, ins::MOD_IMP },                    /* WAI i */
    [0xcc] = { ins::MNE_CPY, ins::MOD_ABS },                    /* CPY a */
    [0xcd] = { ins::MNE_CMP, ins::MOD_ABS },                    /* CMP a */
    [0xce] = { ins::MNE_DEC, ins::MOD_ABS },                    /* DEC a */
    [0xcf] = { ins::MNE_BBS4, ins::MOD_PCR },                   /* BBS4 r */
    [0xd0] = { ins::MNE_BNE, ins::MOD_PCR },                    /* BNE r */
    [0xd1] = { ins::MNE_CMP, ins::MOD_ZPG_IND_Y },              /* CMP (zp),y */
    [0xd2] = { ins::MNE_CMP, ins::MOD_ZPG_IND },                /* CMP (zp) */
    [0xd3] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xd4] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xd5] = { ins::MNE_CMP, ins::MOD_ZPG_X },                  /* CMP zp,x */
    [0xd6] = { ins::MNE_DEC, ins::MOD_ZPG_X },                  /* DEC zp,x */
    [0xd7] = { ins::MNE_SMB5, ins::MOD_ZPG },                   /* SMB5 zp */
    [0xd8] = { ins::MNE_CLD, ins::MOD_IMP },                    /* CLD i */
    [0xd9] = { ins::MNE_CMP, ins::MOD_ABS_Y },                  /* CMP a,y */
    [0xda] = { ins::MNE_PHX, ins::MOD_STK },                    /* PHX s */
    [0xdb] = { ins::MNE_STP, ins::MOD_IMP },                    /* STP i */
    [0xdc] = { ins::MNE_CMP, ins::MOD_ABS_X },                  /* CMP a,x */
    [0xdd] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xde] = { ins::MNE_DEC, ins::MOD_ABS_X },                  /* DEC a,x */
    [0xdf] = { ins::MNE_BBS5, ins::MOD_PCR },                   /* BBS5 r */
    [0xe0] = { ins::MNE_CPX, ins::MOD_IMM },                    /* CPX # */
    [0xe1] = { ins::MNE_SBC, ins::MOD_ZPG_IND_X },              /* SBC (zp,x) */
    [0xe2] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xe3] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xe4] = { ins::MNE_CPX, ins::MOD_ZPG },                    /* CPX zp */
    [0xe5] = { ins::MNE_SBC, ins::MOD_ZPG },                    /* SBC zp */
    [0xe6] = { ins::MNE_INC, ins::MOD_ZPG },                    /* INC zp */
    [0xe7] = { ins::MNE_SMB6, ins::MOD_ZPG },                   /* SMB6 zp */
    [0xe8] = { ins::MNE_INX, ins::MOD_IMP },                    /* INX i */
    [0xe9] = { ins::MNE_SBC, ins::MOD_IMM },                    /* SBC # */
    [0xea] = { ins::MNE_NOP, ins::MOD_IMP },                    /* NOP i */
    [0xeb] = { ins::MNE_CPX, ins::MOD_ABS },                    /* CPX a */
    [0xec] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xed] = { ins::MNE_SBC, ins::MOD_ABS },                    /* SBC a */
    [0xee] = { ins::MNE_INC, ins::MOD_ABS },                    /* INC a */
    [0xef] = { ins::MNE_BBS6, ins::MOD_PCR },                   /* BBS6 r */
    [0xf0] = { ins::MNE_BEQ, ins::MOD_PCR },                    /* BEQ r */
    [0xf1] = { ins::MNE_SBC, ins::MOD_ZPG_IND_Y },              /* SBC (zp),y */
    [0xf2] = { ins::MNE_SBC, ins::MOD_ZPG_IND },                /* SBC (zp) */
    [0xf3] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xf4] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xf5] = { ins::MNE_SBC, ins::MOD_ZPG_X },                  /* SBC zp,x */
    [0xf6] = { ins::MNE_INC, ins::MOD_ZPG_X },                  /* INC zp,x */
    [0xf7] = { ins::MNE_SMB7, ins::MOD_ZPG },                   /* SMB7 zp */
    [0xf8] = { ins::MNE_SED, ins::MOD_IMP },                    /* SED i */
    [0xf9] = { ins::MNE_SBC, ins::MOD_ABS_Y },                  /* SBC a,y */
    [0xfa] = { ins::MNE_PLX, ins::MOD_STK },                    /* PLX s */
    [0xfb] = { ins::MNE_SBC, ins::MOD_ABS_X },                  /* SBC a,x */
    [0xfc] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xfd] = { ins::MNE_INVL, ins::MOD_INV },                   /* invalid */
    [0xfe] = { ins::MNE_INC, ins::MOD_ABS_X },                  /* INC a,x */
    [0xff] = { ins::MNE_BBS7, ins::MOD_PCR },                   /* BBS7 r */
#endif
