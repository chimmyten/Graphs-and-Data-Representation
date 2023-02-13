all: tenth

tenth: tenth.c
	gcc -Wall -Werror -fsanitize=address -g tenth.c -o tenth -lm

clean:
	rm -rf tenth