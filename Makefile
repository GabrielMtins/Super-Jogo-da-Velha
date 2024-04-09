all:
	gcc src/*.c -std='c99' -Wall -Wextra -lcurses -o main

clean:
	rm terminoo
