adboard: main.cc
	g++ -std=c++11 -O2 -o adboard main.cc -lncurses

clean:
	rm -f adboard
