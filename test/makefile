CC = ./dhbwcc
LD = cat

CRT = rt/crt.s
LIBC = rt/libc.s
OUT = out

.SUFFICES: 
.SUFFIXES: .asm .s

vpath %.c $(OUT)
vpath %.s $(OUT)

$(OUT)/%.s: %.c
	$(CC) $< -o $@

%.asm: $(OUT)/%.s
	$(LD) $(CRT) $(LIBC) $< > $(OUT)/$@

.PHONY: clean
clean: 
	$(RM) $(OUT)/*
