CFLAGS = -O3
LDFLAGS = -lSDL2 -lm

run: main
	./main

main: main.o gfx.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) $< -c -o $@

clean:
	-rm *.o
	-rm main