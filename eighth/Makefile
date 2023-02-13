all: eighth

eighth: eighth.c
	gcc -Wall -Werror -fsanitize=address -g eighth.c -o eighth -lm

clean:
	rm -rf eighth