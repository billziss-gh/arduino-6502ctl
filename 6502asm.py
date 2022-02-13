# 6502asm.py
# 6502 assembler
#
# Copyright 2022 Bill Zissimopoulos

import argparse, os, sys

__all__ = ["Assembler", "SyntaxError"]

class Assembler:

    if "__main__" == __name__:
        sys.dont_write_bytecode = True
    insmap = __import__("6502asmtab").insmap

    def __init__(self, errfn=None, trcfn=None):
        self.text = bytearray(64 * 1024)
        self.addr = 0
        self.minaddr = 0xffff
        self.maxaddr = 0
        self.defs = {}
        self.labels = {}
        self.fixups = {}
        self.lastfix = ""
        self.file = ""
        self.line = 0
        self.errfn = warn if errfn is None else errfn
        self.trcfn = trcfn

    def assemble(self, srcfiles, dstfile):
        res = True
        for srcfile in srcfiles:
            try:
                f = open(srcfile, "r")
                self.file = srcfile
                srcfile = f
            except TypeError:
                self.file = "<NONE>"
            res = self.translate(srcfile) and res
        try:
            self.fixup()
        except SyntaxError as ex:
            self.__error(ex)
            res = False
        if not res:
            return

        if self.trcfn:
            self.trcfn("rng: %04x %04x" % (self.minaddr, self.maxaddr))

        try:
            f = open(dstfile, "wb")
            dstfile = f
        except TypeError:
            pass
        self.emit(dstfile)

    def translate(self, srcfile):
        res = True
        self.line = 0
        for line in srcfile:
            try:
                self.line += 1
                line = line.strip()
                line = line.upper()
                part = line.split(";", maxsplit=1)
                part = part[0].split()
                if part:
                    if part[0].endswith(":"):
                        self.addlabel(part[0][:-1], self.addr)
                        part = part[1:]
                if part:
                    if part[0].startswith("."):
                        self.directive(part[0], part[1:])
                    else:
                        self.instruction(part[0], part[1:])
            except SyntaxError as ex:
                self.__error(ex)
                res = False
        return res

    def directive(self, dir, args):
        if ".DEF" == dir:
            if args:
                self.adddef(args[0], " ".join(args[1:]))
        elif ".BYTE" == dir:
            for arg in args:
                val = self.__avalue(arg)
                if 0x10000 == val:
                    self.__addfixup(self.addr, 1)
                self.__text(val, 1)
        elif ".WORD" == dir:
            for arg in args:
                val = self.__avalue(arg)
                if 0x10000 == val:
                    self.__addfixup(self.addr, 2)
                self.__text(val, 2)
        elif ".ORG" == dir:
            self.addr = self.__ivalue(" ".join(args))
            self.minaddr = min(self.minaddr, self.addr)
            self.maxaddr = max(self.maxaddr, self.addr)
            pass
        else:
            raise SyntaxError("invalid directive: %r" % dir)

    def instruction(self, opc, args):
        arg = "".join(args)
        mod, val = self.__amode(arg)
        key = "%s %s" % (opc, mod)
        ins = self.insmap.get(key)
        if not ins:
            # also try r mode
            mod = "r"
            key = "%s %s" % (opc, mod)
            ins = self.insmap.get(key)
        if ins:
            if self.trcfn:
                self.trcfn("ins: %04x %s %x" % (self.addr, key, val))
            if 0x10000 == val:
                self.__addfixup(self.addr + 1, ins[1] - 1 if "r" != mod else -ins[1] + 1)
            self.__text(ins[0])
            if 1 < ins[1]:
                self.__text(val, ins[1] - 1)
        else:
            if arg:
                raise SyntaxError("invalid instruction: %r" % (opc + " " + arg))
            else:
                raise SyntaxError("invalid instruction: %r" % opc)

    def adddef(self, name, val):
        if not name.isidentifier():
            raise SyntaxError("invalid def: %r" % name)
        if self.trcfn:
            self.trcfn("def: %s %s" % (name, val))
        self.defs[name] = val

    def addlabel(self, name, addr):
        if not name.isidentifier():
            raise SyntaxError("invalid label: %r" % name)
        if self.trcfn:
            self.trcfn("lbl: %04x %s" % (addr, name))
        self.labels[name] = addr

    def fixup(self):
        for addr, fix in self.fixups.items():
            self.file, self.line, label, count = fix
            val = self.labels.get(label)
            if val is None:
                raise SyntaxError("invalid label: %r" % label)
            if self.trcfn:
                self.trcfn("fix: %04x %s %s %x" % (addr, label, count, val))
            if 1 == count:
                self.text[addr] = val & 0xff
            elif -1 == count:
                self.text[addr] = (val - addr - 1) & 0xff
            elif 2 == count:
                self.text[addr] = val & 0xff
                self.text[addr + 1] = (val >> 8) & 0xff

    def emit(self, dstfile):
        for addr in range(self.minaddr, self.maxaddr + 1):
            if 0 != (addr - self.minaddr + 1) % 8:
                dstfile.write(b"0x%02x, " % self.text[addr])
            else:
                dstfile.write(b"0x%02x,\n" % self.text[addr])

    def __amode(self, arg):
        s = self.__getdef(arg)
        if not s:
            return ".", 0
        elif s.startswith("#"):
            return "#", self.__ivalue(s[1:])
        elif s.startswith("("):
            if s.endswith(",X)"):
                s = s[1:-3]
                v = self.__avalue(s)
                zpg = 0xff >= v and (3 >= len(s) or "$" != s[0])
                return ("(zp,x)" if zpg else "(a,x)"), v
            elif s.endswith(")"):
                s = s[1:-1]
                v = self.__avalue(s)
                zpg = 0xff >= v and (3 >= len(s) or "$" != s[0])
                return ("(zp)" if zpg else "(a)"), v
            elif s.endswith("),Y"):
                s = s[1:-3]
                v = self.__avalue(s)
                return "(zp),y", v
            else:
                raise SyntaxError("invalid operand: %r", arg)
        else:
            if s.endswith(",X"):
                s = s[:-2]
                v = self.__avalue(s)
                zpg = 0xff >= v and (3 >= len(s) or "$" != s[0])
                return ("zp,x" if zpg else "a,x"), v
            if s.endswith(",Y"):
                s = s[:-2]
                v = self.__avalue(s)
                zpg = 0xff >= v and (3 >= len(s) or "$" != s[0])
                return ("zp,y" if zpg else "a,y"), v
            else:
                v = self.__avalue(s)
                zpg = 0xff >= v and (3 >= len(s) or "$" != s[0])
                return ("zp" if zpg else "a"), v

    def __avalue(self, arg):
        s = self.__getdef(arg)
        if s.isidentifier():
            self.lastfix = s
            return 0x10000
        return self.__rawvalue(arg, s)

    def __ivalue(self, arg):
        s = self.__getdef(arg)
        return self.__rawvalue(arg, s)

    def __rawvalue(self, arg, s):
        try:
            if s.startswith("$"):
                return int(s[1:], 16)
            else:
                return int(s, 10)
        except ValueError:
            raise SyntaxError("invalid value: %r" % arg)

    def __getdef(self, arg):
        s = arg
        for _ in range(16):
            if not s.isidentifier():
                break
            t = self.defs.get(s, "")
            if not t:
                break
            s = t
        return s

    def __addfixup(self, addr, count):
        if self.trcfn:
            self.trcfn("mrk: %04x %s %s" % (addr, self.lastfix, count))
        self.fixups[addr] = (self.file, self.line, self.lastfix, count)

    def __text(self, val, count=1):
        self.minaddr = min(self.minaddr, self.addr)
        for i in range(count):
            if self.trcfn:
                self.trcfn("txt: %04x %02x" % (self.addr + i, val & 0xff))
            self.text[self.addr + i] = val & 0xff
            val >>= 8
        self.addr += count
        self.maxaddr = max(self.maxaddr, self.addr - 1)

    def __error(self, ex):
        self.errfn("%s:%s: error: %s" % (self.file, self.line, ex))

class SyntaxError(Exception):
    pass

def main():
    p = argparse.ArgumentParser(add_help=False)
    p.add_argument("-o", dest="outfile", default=sys.stdout.buffer, help="output file")
    p.add_argument("-v", dest="verbose", action="store_true", help="verbose output")
    p.add_argument("srcfile", nargs="+", help="assembly file")
    args = p.parse_args(sys.argv[1:])
    asm = Assembler(trcfn = (lambda s: print(s, file=sys.stderr)) if args.verbose else None)
    asm.assemble(args.srcfile, args.outfile)

def warn(s):
    print("%s: %s" % (os.path.basename(sys.argv[0]), s), file=sys.stderr)
def fail(s, exitcode = 1):
    warn(s)
    sys.exit(exitcode)

def __entry():
    try:
        main()
    except EnvironmentError as ex:
        fail(ex)
    except KeyboardInterrupt:
        fail("interrupted", 130)

if "__main__" == __name__:
    __entry()
