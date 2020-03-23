ARCH=mips-linux-gnu

CC=$(ARCH)-gcc
AS=$(ARCH)-as
LD=$(ARCH)-ld
OBJCOPY=$(ARCH)-objcopy

#ARCHFLAGS += -march=mips3
ARCHFLAGS += -fno-lto
#ARCHFLAGS += -fno-lto  -static -specs=/opt/musl/mips-linux-gnu/lib/musl-gcc.specs
#ARCHFLAGS += -mno-shared

CFLAGS  += -ggdb -O3
AFLAGS  += -ggdb
LDFLAGS += -ggdb
LDFLAGS += -nostartfiles
LDFLAGS += -static
#LDFLAGS += -Wl,-Ttext,0x1000

CFLAGS  += $(ARCHFLAGS)
AFLAGS  += $(ARCHFLAGS)
LDFLAGS += $(ARCHFLAGS)

all:default

.PHONY:clean

%.srec:%
	$(OBJCOPY) -O srec $< $@

%.out:%
	$(OBJCOPY) -O ecoff-bigmips $< $@

%.o:%.S
	$(CC) -D__ASSEMBLY__ $(AFLAGS) -c $< -o $@

%.s:%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -S $< -o $@

%.o:%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@


default : dial-test.srec

dial-test : crt0local.o dial-test.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f serial-port-test *.o

#mips-elf-objdump --source -M no-aliases,reg-names=numeric exam1-asm
