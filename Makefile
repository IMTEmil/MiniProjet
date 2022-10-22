snake: snake.c projet.c
	cc -o snake projet.c snake.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 

game: snake
	./snake
