CFLAGS = -O3
LDFLAGS = -lSDL2 -lm

run: main
	./main

gdb: gdbcompile
	gdb main

main: main.o gfx.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

gdbcompile: main.og gfx.og
	gcc -g $^ -o $@ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) $< -c -o $@

%.og: %.c
	gcc -g $< -c -o $@


clean:
	-rm *.o
	-rm main