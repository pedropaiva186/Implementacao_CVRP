all: create

create:
	g++ *.cpp -o code -O3

run:
	./code

clean:
	rm ./code *.txt