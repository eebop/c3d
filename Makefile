CFLAGS = -O3 -Wall -Wextra
LDFLAGS = -lSDL2 -lm

run: main
	./main

gdb: gdbcompile
	gdb gdbcompile

main: main.o gfx.o quaternion.o events.o abstraction.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

gdbcompile: main.og gfx.og quaternion.og events.og abstraction.og
	gcc -g $^ -o $@ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) $< -c -o $@

%.og: %.c
	gcc -g $< -c -o $@


clean:
	-rm *.o
	-rm *.og
	-rm main