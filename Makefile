CC=gcc
CFLAGS=-Wall
LIBS=-lm
NAME= ray

make: $(NAME).c
	$(CC) -o $(NAME) *.c $(LIBS) 

clean: $(NAME)
	rm $(NAME)
