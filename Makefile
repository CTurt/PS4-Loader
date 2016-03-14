LIBPS4	:=	$(PS4SDK)/libPS4

DATA	:=	0x926300000

CC		:=	gcc
AS		:=	gcc
OBJCOPY	:=	objcopy
ODIR	:=	build
SDIR	:=	source
IDIRS	:=	-I$(LIBPS4)/include -I. -Iinclude
LDIRS	:=	-L$(LIBPS4) -L. -Llib
CFLAGS	:=	$(IDIRS) -O2 -std=gnu11 -fno-builtin -nostartfiles -nostdlib -Wall -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -DDATA_ADDRESS=$(DATA)
SFLAGS	:=	-nostartfiles -nostdlib -march=btver2 -mtune=btver2
LFLAGS	:=	$(LDIRS)
CFILES	:=	$(wildcard $(SDIR)/*.c)
SFILES	:=	$(wildcard $(SDIR)/*.s)
OBJS	:=	$(patsubst $(SDIR)/%.c, build/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, build/%.o, $(SFILES))

LIBS	:=	

TARGET = $(shell basename $(CURDIR))

$(TARGET): $(ODIR) $(OBJS)
	$(CC) $(ODIR)/*.o -o temp.t $(CFLAGS) $(LFLAGS) $(LIBS)
	$(OBJCOPY) -O binary temp.t $(TARGET)
	rm -f temp.t

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(ODIR)/*.o
