all: create

create:
	g++ *.cpp lib/*.cpp -o code -O3

run:
	./code

clean:
	rm ./code *.txt