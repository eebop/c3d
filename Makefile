GLOBALCFLAGS = -O3 -Wall -Wextra
WINCFLAGS = -ISDL/Windows/include
GLOBALCFLAGS = -lm -march=native -Wpsabi
LINUXLDFLAGS = -lSDL2
WINLDFLAGS = -w -LSDL/windows/lib -l:libSDL2.a -mwindows -lole32 -loleaut32 -limm32 -lwinmm -lversion -lsetupapi# -lgdi32#-Wl,-subsystem,windows 

CC = gcc
run: linux
	./main

windows: CFLAGS =  $(GLOBALCFLAGS)  $(WINCFLAGS)
windows: LDFLAGS = $(GLOBALLDFLAGS) $(WINLDFLAGS)
windows: main

linux: CFLAGS =  $(GLOBALCFLAGS)
linux: LDFLAGS = $(GLOBALLDFLAGS) $(LINUXLDFLAGS)
linux: main

wintest: CFLAGS =  $(GLOBALCFLAGS) $(WINCFLAGS)
wintest: LDFLAGS = $(GLOBALLDFLAGS) $(WINLDFLAGS)
wintest: test.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

main: main.o gfx.o quaternion.o events.o abstraction.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) $< -c -o $@

clean:
	-rm *.o
	-rm main