1. Observe that the very last expression in `tracem2.c` is `output[i] = '\0';`
2. `objdump -D -M intel traceme > traceme.S`
3. Suspect that `8bc: b8 00 00 00 00 mov eax,0x0` might be the aforementioned expression in assembly form.
4. Run the program in gdb and set a break point @8bc
5. Bingo ![img](https://i.imgur.com/bd80eOA.png)
