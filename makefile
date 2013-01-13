CC = gcc
MAIN_BINARIES = $(basename $(wildcard *Main.cpp))

#CFLAGS  = -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -marmv6z -Wall
CFLAGS  = -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -marm -O3 -Wall
LD = ld
LDFLAGS = -lrt -lstdc++


OBJ = bcm2835.o lcd.o raspilcd.o Display.o
BIN = raspilcd

gpio: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ) $(MAIN_BINARIES)

compile: $(MAIN_BINARIES)

%Main: %Main.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	
