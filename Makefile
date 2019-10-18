build_command = g++ -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP -Isrc -I$INCLUDE_PATH -L$LIB_PATH -lraylib

game.exe: build_dir build/main.o build/ball.o build/player.o build/bot.o build/utils.o
	$(build_command) -o build/game.exe build/*.o

build_dir:
	mkdir -p build

build/main.o: src/main.cpp
	$(build_command) -c -o build/main.o src/main.cpp

build/ball.o: src/ball.cpp src/ball.h src/utils.h
	$(build_command) -c -o build/ball.o src/ball.cpp

build/player.o: src/player.cpp src/player.h src/utils.h
	$(build_command) -c -o build/player.o src/player.cpp

build/bot.o: src/bot.cpp src/bot.h
	$(build_command) -c -o build/bot.o src/bot.cpp

build/utils.o: src/utils.cpp src/utils.h
	$(build_command) -c -o build/utils.o src/utils.cpp

clean:
	rm -rf build
