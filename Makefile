build_command = clang++ -g -std=c++17 -Wall -Wextra -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP -I./src/ -I$INCLUDE_PATH -L$LIB_PATH -lraylib

game.exe: build_dir build/main.o build/ball.o build/player.o build/bot.o build/sounds.o
	$(build_command) -o build/game.exe build/*.o

build_dir:
	mkdir -p build

build/main.o: src/main.cpp src/collisions.h src/graphics.h src/utils.h
	$(build_command) -c -o build/main.o src/main.cpp

build/ball.o: src/ball.cpp src/ball.h
	$(build_command) -c -o build/ball.o src/ball.cpp

build/player.o: src/player.cpp src/player.h
	$(build_command) -c -o build/player.o src/player.cpp

build/bot.o: src/bot.cpp src/bot.h
	$(build_command) -c -o build/bot.o src/bot.cpp

build/sounds.o: src/sounds.cpp src/sounds.h
	$(build_command) -c -o build/sounds.o src/sounds.cpp

clean:
	rm -rf build
