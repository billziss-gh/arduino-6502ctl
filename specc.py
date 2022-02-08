ilst = []
with open("6502ins.spec", "rb") as file:
    for line in file:
        line = line.strip()
        part = line.split()
        ilst.append((int(part[0], 0), part[1], part[2], int(part[3], 0)))

mset = set()
for i in ilst:
    mset.add(i[1])

modmap = {
    b"INV": b"INV",
    b"a": b"ABS",
    b"(a,x)": b"ABS_IND_X",
    b"a,x": b"ABS_X",
    b"a,y": b"ABS_Y",
    b"(a)": b"ABS_IND",
    b"A": b"ACC",
    b"#": b"IMM",
    b"i": b"IMP",
    b"r": b"PCR",
    b"s": b"STK",
    b"zp": b"ZPG",
    b"(zp,x)": b"ZPG_IND_X",
    b"zp,x": b"ZPG_X",
    b"zp,y": b"ZPG_Y",
    b"(zp)": b"ZPG_IND",
    b"(zp),y": b"ZPG_IND_Y",
}

with open("6502distab.h", "wb") as file:
    file.write(b"#if defined(DISTAB_MNE_ENUM)\n")
    i = 0
    for mnem in sorted(mset):
        file.write(b"    MNE_%s = %i,\n" % (mnem, i))
        i += 1
    file.write(b"#elif defined(DISTAB_MNE_STR)\n")
    i = 0
    for mnem in sorted(mset):
        file.write(b"    \"%s\",\n" % mnem)
        i += 1
    file.write(b"#elif defined(DISTAB_INS)\n")
    for i in ilst:
        defn = b"[0x%02x] = { ins::MNE_%s, ins::MOD_%s }," % (i[0], i[1], modmap[i[2]])
        cmnt = b"/* %s %s */" % (i[1], i[2]) if b"INVL" != i[1] else b"/* invalid */"
        file.write(b"    %-60s%s\n" % (defn, cmnt))
    file.write(b"#endif\n")

with open("6502astab.py", "wb") as file:
    file.write(b"insmap = {\n")
    for i in ilst:
        if b"INVL" == i[1]:
            continue
        m = i[2]
        if 1 == i[3]:
            m = b"."
        out0 = b"\"%s %s\":" % (i[1], m)
        out1 = b"(0x%02x, %d)," % (i[0], i[3])
        file.write(b"    %-16s%s\n" % (out0, out1))
    file.write(b"}\n")
