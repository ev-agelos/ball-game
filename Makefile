build_command = clang++ -g -std=c++17 -Wall -Wextra -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP -isystem -I./src/

game.exe: build_dir build/main.o build/ball.o build/player.o build/bot.o build/sounds.o
	$(build_command) -lraylib -o build/game.exe build/*.o

build_dir:
	mkdir -p build

build/main.o: src/main.cpp src/collisions.hpp src/graphics.hpp src/utils.hpp
	$(build_command) -c -o build/main.o src/main.cpp

build/ball.o: src/ball.cpp src/ball.hpp
	$(build_command) -c -o build/ball.o src/ball.cpp

build/player.o: src/player.cpp src/player.hpp
	$(build_command) -c -o build/player.o src/player.cpp

build/bot.o: src/bot.cpp src/bot.hpp
	$(build_command) -c -o build/bot.o src/bot.cpp

build/sounds.o: src/sounds.cpp src/sounds.hpp
	$(build_command) -c -o build/sounds.o src/sounds.cpp

clean:
	rm -rf build
