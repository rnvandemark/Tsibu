all: bin/Tsibu.o

bin/Tsibu.o: main/TsibuMain.cpp
	mkdir -p bin/
	g++ -pthread main/TsibuMain.cpp src/common/src/GPIORegistrar.cpp src/common/src/GPIO.cpp src/hunger/src/HungerLevelController.cpp src/hunger/src/HungerChangeMagnitudeController.cpp src/operation_mode/src/CurrentOperationModeController.cpp src/motor_control/src/CurrentMovementDirectionController.cpp src/common/src/SerialCommunicator.cpp src/common/src/I2CCommunicator.cpp src/motor_control/src/ThunderBorgController.cpp -I src/common/include/,src/hunger/include/,src/operation_mode/include/,src/motor_control/include/ -std=c++11 -o bin/Tsibu.o

clean:
	rm -f -r bin/

fresh: clean bin/Tsibu.o
