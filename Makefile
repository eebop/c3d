GLOBALCFLAGS = -O3
WINCFLAGS = -I/cygdrive/c/Users/kiera/Downloads/SDL2-devel-2.28.3-mingw/SDL2-2.28.3/x86_64-w64-mingw32/include
LINUXLDFLAGS = -lSDL2 -lm
WINLDFLAGS = -L/cygdrive/c/Users/kiera/Downloads/SDL2-devel-2.28.3-mingw/SDL2-2.28.3/x86_64-w64-mingw32/lib -w -lm -l:libSDL2.a -Wl,-subsystem,windows -lole32 -loleaut32 -limm32 -lwinmm -lversion -lsetupapi -lgdi32

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

gdb: gdbcompile
	gdb main

gdbcompile: main.og gfx.og quaternion.og events.og abstraction.og
	gcc -g $^ -o $@ $(LDFLAGS) $(LDFLAGSPART2)

%.o: %.c
	gcc $(CFLAGS) $< -c -o $@

%.og: %.c
	gcc -g $< -c -o $@

test: test.o
	gcc $(CFLAGS) test.o -o test $(LDFLAGS)
	./test

clean:
	-rm*.o
	-rm *.og
	-rm main