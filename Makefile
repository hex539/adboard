CXXFLAGS = $(shell pkg-config --libs ncurses)

adboard: main.cc
	g++ -std=c++11 -O2 -pthread -o adboard main.cc $(CXXFLAGS)

clean:
	rm -f adboard
