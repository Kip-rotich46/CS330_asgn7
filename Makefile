FILE = cs330_sokoban_game
CFLAGS = -Wall -g

build: $(FILE).c
	gcc -Wall -g $(FILE).c -L. -lsok_helper_vulcan -o $(FILE) -lncurses -lm -fno-pie -no-pie

.PHONY: db

db:
	gdb -tui $(FILE)

run:
	./$(FILE)
