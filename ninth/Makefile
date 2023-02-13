all: ninth

ninth: ninth.c
	gcc -Wall -Werror -fsanitize=address -g ninth.c -o ninth -lm

clean:
	rm -rf ninth