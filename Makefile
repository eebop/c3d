GLOBALCFLAGS = -O3 -Wall -Wextra
WINCFLAGS = -ISDL/Windows/include
LINUXLDFLAGS = -lSDL2 -lm
WINLDFLAGS = -w -lm -LSDL/windows/lib -l:libSDL2.a -mwindows -lole32 -loleaut32 -limm32 -lwinmm -lversion -lsetupapi# -lgdi32#-Wl,-subsystem,windows 

CC = gcc
run: linux
	./main

windows: CFLAGS = $(GLOBALCFLAGS) $(WINCFLAGS)
windows: LDFLAGS = $(WINLDFLAGS)
windows: main

linux: CFLAGS = $(GLOBALCFLAGS)
linux: LDFLAGS = $(LINUXLDFLAGS)
linux: main

main: main.o gfx.o quaternion.o events.o abstraction.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDFLAGSPART2)

%.o: %.c
	gcc $(CFLAGS) $< -c -o $@

clean:
	-rm *.o
	-rm *.og
	-rm main