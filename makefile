CC = gcc
MAIN_BINARIES = $(basename $(wildcard *Main.cpp))

#CFLAGS  = -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -marmv6z -Wall
CFLAGS  = -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -marm -O3 -Wall
CXXFLAGS = -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -marm -O3 -std=c++11 -Wall
LD = ld
LDFLAGS = -lrt -lstdc++ -lboost_system  -lboost_filesystem -lmpg123 -lao

OBJ = bcm2835.o RaspiLCD.o Library.o lcd.o raspilcd.o JukeBerry.o Player.o Screen.o LibraryBrowserScreen.o MainMenuScreen.o ContextMenuScreen.o

all: compile

.PHONY: clean
clean:
	rm -rf $(OBJ) $(MAIN_BINARIES) RaspiLCDMain.o

RaspiLCDMain: RaspiLCDMain.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

compile: $(MAIN_BINARIES)
