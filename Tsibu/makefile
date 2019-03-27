all: bin/Tsibu.o

bin/Tsibu.o: main/TsibuMain.cpp
	mkdir -p bin/
	g++ main/TsibuMain.cpp -I include/ -std=c++11 -o bin/Tsibu.o

clean:
	rm -f -r bin/

fresh: clean bin/Tsibu.o