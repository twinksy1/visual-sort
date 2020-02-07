all: sort

sort: sort.cpp
	g++ sort.cpp -Wall -o sort -lX11 -lXext -lm

clean:
	rm -f sort
