# 6502as.py
# 6502 assembler
#
# Copyright 2022 Bill Zissimopoulos

import sys

insmap = {
    "BRK .":                            (0x00, 1),
    "ORA (zp,x)":                       (0x01, 2),
    "TSB zp":                           (0x04, 2),
    "ORA zp":                           (0x05, 2),
    "ASL zp":                           (0x06, 2),
    "RMB0 zp":                          (0x07, 2),
    "PHP .":                            (0x08, 1),
    "ORA #":                            (0x09, 2),
    "ASL .":                            (0x0a, 1),
    "TSB a":                            (0x0c, 3),
    "ORA a":                            (0x0d, 3),
    "ASL a":                            (0x0e, 3),
    "BBR0 r":                           (0x0f, 2),
    "BPL r":                            (0x10, 2),
    "ORA (zp),y":                       (0x11, 2),
    "ORA (zp)":                         (0x12, 2),
    "TRB zp":                           (0x14, 2),
    "ORA zp,x":                         (0x15, 2),
    "ASL zp,x":                         (0x16, 2),
    "RMB1 zp":                          (0x17, 2),
    "CLC .":                            (0x18, 1),
    "ORA a,y":                          (0x19, 3),
    "INC .":                            (0x1a, 1),
    "TRB a":                            (0x1c, 3),
    "ORA a,x":                          (0x1d, 3),
    "ASL a,x":                          (0x1e, 3),
    "BBR1 r":                           (0x1f, 2),
    "JSR a":                            (0x20, 3),
    "AND (zp,x)":                       (0x21, 2),
    "BIT zp":                           (0x24, 2),
    "AND zp":                           (0x25, 2),
    "ROL zp":                           (0x26, 2),
    "RMB2 zp":                          (0x27, 2),
    "PLP .":                            (0x28, 1),
    "AND #":                            (0x29, 2),
    "ROL .":                            (0x2a, 1),
    "BIT a":                            (0x2c, 3),
    "AND a":                            (0x2d, 3),
    "ROL a":                            (0x2e, 3),
    "BBR2 r":                           (0x2f, 2),
    "BMI r":                            (0x30, 2),
    "AND (zp),y":                       (0x31, 2),
    "AND (zp)":                         (0x32, 2),
    "BIT zp,x":                         (0x34, 2),
    "AND zp,x":                         (0x35, 2),
    "ROL zp,x":                         (0x36, 2),
    "RMB3 zp":                          (0x37, 2),
    "SEC .":                            (0x38, 1),
    "AND a,y":                          (0x39, 3),
    "DEC .":                            (0x3a, 1),
    "BIT a,x":                          (0x3c, 3),
    "AND a,x":                          (0x3d, 3),
    "ROL a,x":                          (0x3e, 3),
    "BBR3 r":                           (0x3f, 2),
    "RTI .":                            (0x40, 1),
    "EOR (zp,x)":                       (0x41, 2),
    "EOR zp":                           (0x45, 2),
    "LSR zp":                           (0x46, 2),
    "RMB4 zp":                          (0x47, 2),
    "PHA .":                            (0x48, 1),
    "EOR #":                            (0x49, 2),
    "LSR .":                            (0x4a, 1),
    "JMP a":                            (0x4c, 3),
    "EOR a":                            (0x4d, 3),
    "LSR a":                            (0x4e, 3),
    "BBR4 r":                           (0x4f, 2),
    "BVC r":                            (0x50, 2),
    "EOR (zp),y":                       (0x51, 2),
    "EOR (zp)":                         (0x52, 2),
    "EOR zp,x":                         (0x55, 2),
    "LSR zp,x":                         (0x56, 2),
    "RMB5 zp":                          (0x57, 2),
    "CLI .":                            (0x58, 1),
    "EOR a,y":                          (0x59, 3),
    "PHY .":                            (0x5a, 1),
    "EOR a,x":                          (0x5d, 3),
    "LSR a,x":                          (0x5e, 3),
    "BBR5 r":                           (0x5f, 2),
    "RTS .":                            (0x60, 1),
    "ADC (zp,x)":                       (0x61, 2),
    "STZ zp":                           (0x64, 2),
    "ADC zp":                           (0x65, 2),
    "ROR zp":                           (0x66, 2),
    "RMB6 zp":                          (0x67, 2),
    "PLA .":                            (0x68, 1),
    "ADC #":                            (0x69, 2),
    "ROR .":                            (0x6a, 1),
    "JMP (a)":                          (0x6c, 3),
    "ADC a":                            (0x6d, 3),
    "ROR a":                            (0x6e, 3),
    "BBR6 r":                           (0x6f, 2),
    "BVS r":                            (0x70, 2),
    "ADC (zp),y":                       (0x71, 2),
    "ADC (zp)":                         (0x72, 2),
    "STZ zp,x":                         (0x74, 2),
    "ADC zp,x":                         (0x75, 2),
    "ROR zp,x":                         (0x76, 2),
    "RMB7 zp":                          (0x77, 2),
    "SEI .":                            (0x78, 1),
    "ADC a,y":                          (0x79, 3),
    "PLY .":                            (0x7a, 1),
    "JMP (a,x)":                        (0x7c, 3),
    "ADC a,x":                          (0x7d, 3),
    "ROR a,x":                          (0x7e, 3),
    "BBR7 r":                           (0x7f, 2),
    "BRA r":                            (0x80, 2),
    "STA (zp,x)":                       (0x81, 2),
    "STY zp":                           (0x84, 2),
    "STA zp":                           (0x85, 2),
    "STX zp":                           (0x86, 2),
    "SMB0 zp":                          (0x87, 2),
    "DEY .":                            (0x88, 1),
    "BIT #":                            (0x89, 2),
    "TXA .":                            (0x8a, 1),
    "STY a":                            (0x8c, 3),
    "STA a":                            (0x8d, 3),
    "STX a":                            (0x8e, 3),
    "BBS0 r":                           (0x8f, 2),
    "BCC r":                            (0x90, 2),
    "STA (zp),y":                       (0x91, 2),
    "STA (zp)":                         (0x92, 2),
    "STY zp,x":                         (0x94, 2),
    "STA zp,x":                         (0x95, 2),
    "STX zp,y":                         (0x96, 2),
    "SMB1 zp":                          (0x97, 2),
    "TYA .":                            (0x98, 1),
    "STA a,y":                          (0x99, 3),
    "TXS .":                            (0x9a, 1),
    "STZ a":                            (0x9c, 3),
    "STA a,x":                          (0x9d, 3),
    "STZ a,x":                          (0x9e, 3),
    "BBS1 r":                           (0x9f, 2),
    "LDY #":                            (0xa0, 2),
    "LDA (zp,x)":                       (0xa1, 2),
    "LDX #":                            (0xa2, 2),
    "LDY zp":                           (0xa4, 2),
    "LDA zp":                           (0xa5, 2),
    "LDX zp":                           (0xa6, 2),
    "SMB2 zp":                          (0xa7, 2),
    "TAY .":                            (0xa8, 1),
    "LDA #":                            (0xa9, 2),
    "TAX .":                            (0xaa, 1),
    "LDY a":                            (0xac, 3),
    "LDA a":                            (0xad, 3),
    "LDX a":                            (0xae, 3),
    "BBS2 r":                           (0xaf, 2),
    "BCS r":                            (0xb0, 2),
    "LDA (zp),y":                       (0xb1, 2),
    "LDA (zp)":                         (0xb2, 2),
    "LDY zp,x":                         (0xb4, 2),
    "LDA zp,x":                         (0xb5, 2),
    "LDX zp,y":                         (0xb6, 2),
    "SMB3 zp":                          (0xb7, 2),
    "CLV .":                            (0xb8, 1),
    "LDA a,y":                          (0xb9, 3),
    "TSX .":                            (0xba, 1),
    "LDY a,x":                          (0xbc, 3),
    "LDA a,x":                          (0xbd, 3),
    "LDX a,y":                          (0xbe, 3),
    "BBS3 r":                           (0xbf, 2),
    "CPY #":                            (0xc0, 2),
    "CMP (zp,x)":                       (0xc1, 2),
    "CPY zp":                           (0xc4, 2),
    "CMP zp":                           (0xc5, 2),
    "DEC zp":                           (0xc6, 2),
    "SMB4 zp":                          (0xc7, 2),
    "INY .":                            (0xc8, 1),
    "CMP #":                            (0xc9, 2),
    "DEX .":                            (0xca, 1),
    "WAI .":                            (0xcb, 1),
    "CPY a":                            (0xcc, 3),
    "CMP a":                            (0xcd, 3),
    "DEC a":                            (0xce, 3),
    "BBS4 r":                           (0xcf, 2),
    "BNE r":                            (0xd0, 2),
    "CMP (zp),y":                       (0xd1, 2),
    "CMP (zp)":                         (0xd2, 2),
    "CMP zp,x":                         (0xd5, 2),
    "DEC zp,x":                         (0xd6, 2),
    "SMB5 zp":                          (0xd7, 2),
    "CLD .":                            (0xd8, 1),
    "CMP a,y":                          (0xd9, 3),
    "PHX .":                            (0xda, 1),
    "STP .":                            (0xdb, 1),
    "CMP a,x":                          (0xdc, 3),
    "DEC a,x":                          (0xde, 3),
    "BBS5 r":                           (0xdf, 2),
    "CPX #":                            (0xe0, 2),
    "SBC (zp,x)":                       (0xe1, 2),
    "CPX zp":                           (0xe4, 2),
    "SBC zp":                           (0xe5, 2),
    "INC zp":                           (0xe6, 2),
    "SMB6 zp":                          (0xe7, 2),
    "INX .":                            (0xe8, 1),
    "SBC #":                            (0xe9, 2),
    "NOP .":                            (0xea, 1),
    "CPX a":                            (0xeb, 3),
    "SBC a":                            (0xed, 3),
    "INC a":                            (0xee, 3),
    "BBS6 r":                           (0xef, 2),
    "BEQ r":                            (0xf0, 2),
    "SBC (zp),y":                       (0xf1, 2),
    "SBC (zp)":                         (0xf2, 2),
    "SBC zp,x":                         (0xf5, 2),
    "INC zp,x":                         (0xf6, 2),
    "SMB7 zp":                          (0xf7, 2),
    "SED .":                            (0xf8, 1),
    "SBC a,y":                          (0xf9, 3),
    "PLX .":                            (0xfa, 1),
    "SBC a,x":                          (0xfb, 3),
    "INC a,x":                          (0xfe, 3),
    "BBS7 r":                           (0xff, 2),
}

class SyntaxError(Exception):
    pass

def value(arg):
    try:
        if arg.startswith("$"):
            return int(arg[1:], 16)
        else:
            return int(arg[1:], 10)
    except:
        raise SyntaxError("invalid value: %s" % arg)

def mode(arg):
    if not arg:
        return ".", 0
    elif arg.startswith("#"):
        return "#", value(arg[1:])
    elif arg.startswith("("):
        if arg.endswith(",X)"):
            a = arg[1:-3]
            v = value(a)
            zpg = 0xff >= v and (3 >= len(a) or "$" != a[0])
            return ("(zp,x)" if zpg else "(a,x)"), v
        elif arg.endswith(")"):
            a = arg[1:-1]
            v = value(a)
            zpg = 0xff >= v and (3 >= len(a) or "$" != a[0])
            return ("(zp)" if zpg else "(a)"), v
        elif arg.endswith("),Y"):
            a = arg[1:-3]
            v = value(a)
            return "(zp),y", v
        else:
            raise SyntaxError("invalid operand: %s", arg)
    else:
        if arg.endswith(",X"):
            a = arg[:-2]
            v = value(a)
            zpg = 0xff >= v and (3 >= len(a) or "$" != a[0])
            return ("zp,x" if zpg else "a,x"), v
        if arg.endswith(",Y"):
            a = arg[:-2]
            v = value(a)
            zpg = 0xff >= v and (3 >= len(a) or "$" != a[0])
            return ("zp,y" if zpg else "a,y"), v
        else:
            a = arg
            v = value(a)
            zpg = 0xff >= v and (3 >= len(a) or "$" != a[0])
            return ("zp" if zpg else "a"), v

line = 0
minaddr = 0x10000
maxaddr = -1
addr = 0
text = bytearray(64 * 1024)

def directive(dir, arg):
    global minaddr, maxaddr, addr, text
    if ".ORG" == dir:
        addr = value(arg)
        minaddr = min(minaddr, addr)
        maxaddr = max(maxaddr, addr)
    elif ".BYTE" == dir:
        minaddr = min(minaddr, addr)
        val = value(arg)
        text[addr] = val & 0xff
        addr += 1
        maxaddr = max(maxaddr, addr - 1)
    elif ".WORD" == dir:
        minaddr = min(minaddr, addr)
        val = value(arg)
        text[addr + 0] = val & 0xff
        text[addr + 1] = (val >> 8) & 0xff
        addr += 2
        maxaddr = max(maxaddr, addr - 1)
    else:
        raise SyntaxError("invalid directive: %s" % dir)

def assemble(f):
    global line, minaddr, maxaddr, addr, text
    line = 0
    for l in f:
        line += 1
        l = l.strip()
        p = l.split(";")
        if not p[0]:
            continue
        p = p[0].split()
        opc = p[0].upper()
        arg = p[1].upper() if 2 <= len(p) else ""
        if opc.startswith("."):
            directive(opc, arg)
            continue
        mod, val = mode(arg)
        key = "%s %s" % (opc, mod)
        ins = insmap.get(key)
        if not ins and "zp" == mod:
            # zp and r address modes look similar; try both
            mod = "r"
            key = "%s %s" % (opc, mod)
            ins = insmap.get(key)
        if ins:
            minaddr = min(minaddr, addr)
            text[addr] = ins[0]
            if 2 == ins[1]:
                text[addr + 1] = val & 0xff
            elif 3 == ins[1]:
                text[addr + 1] = val & 0xff
                text[addr + 2] = (val >> 8) & 0xff
            addr += ins[1]
            maxaddr = max(maxaddr, addr - 1)
        else:
            raise SyntaxError("invalid instruction: %s" % l)

def emit():
    global minaddr, maxaddr, text
    for addr in range(minaddr, maxaddr + 1):
        print("0x%02x," % text[addr], end=" " if 0 != (addr - minaddr + 1) % 8 else "\n")

def main(args):
    for file in args:
        with open(file, "r") as f:
            try:
                assemble(f)
            except SyntaxError as ex:
                print("%s:%s: error: %s" % (file, line, ex), file=sys.stderr)
    emit()

main(sys.argv[1:])
