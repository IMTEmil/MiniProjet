snake:
	cc -o snake snake.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 

game: snake
	./snake
