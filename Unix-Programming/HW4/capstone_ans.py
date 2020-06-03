from pwn import *
from capstone import *

conn = remote("aup.zoolab.org", 2530)
while True:
    x = conn.recvuntil((">>> ", "}"))
    print(x)

    raw_code = conn.recvline().strip().decode()
    code = []

    for i in range(0, len(raw_code), 2):
        code.append(int(raw_code[i : i + 2], base=16))

    md = Cs(CS_ARCH_X86, CS_MODE_64)
    ans = ""
    for i in md.disasm(bytes(code), 0x0):
        for c in i.mnemonic:
            ans += f"{ord(c):02x}"
        ans += f"{ord(' '):02x}"
        for c in i.op_str:
            ans += f"{ord(c):02x}"
        ans += "{:02x}".format(ord("\n"))
    conn.sendline(ans)
