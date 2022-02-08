# 6502as.py
# 6502 assembler
#
# Copyright 2022 Bill Zissimopoulos

import sys

sys.dont_write_bytecode = True
insmap = __import__("6502astab").insmap

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
