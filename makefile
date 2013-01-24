CC = gcc
MAIN_BINARIES = $(basename $(wildcard *Main.cpp))

#CFLAGS  = -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -marmv6z -Wall
CFLAGS  = -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -marm -O3 -Wall
LD = ld
LDFLAGS = -lrt -lstdc++

OBJ = bcm2835.o RaspiLCD.o

all: compile

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(OBJ) $(MAIN_BINARIES) RaspiLCDMain.o

RaspiLCDMain: RaspiLCDMain.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

compile: $(MAIN_BINARIES)
