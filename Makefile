all:
	gcc -Wall -g -o0 -I/usr/include/SDL2 -I. -o pihole_mon *.c  -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lcurl

clean:
	rm ./pihole_mon