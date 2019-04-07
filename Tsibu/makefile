all: bin/Tsibu.o

bin/Tsibu.o: main/TsibuMain.cpp
	mkdir -p bin/
	g++ -pthread main/TsibuMain.cpp src/hunger/src/HungerLevelController.cpp -I src/common/include/,src/hunger/include/ -std=c++11 -o bin/Tsibu.o

clean:
	rm -f -r bin/

fresh: clean bin/Tsibu.o
