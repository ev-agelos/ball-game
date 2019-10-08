game.exe: build_dir main.o ball.o player.o bot.o utils.o
	g++ -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP -Isrc -I$INCLUDE_PATH -L$LIB_PATH -lraylib -o build/game.exe build/*.o

build_dir:
	mkdir -p build

main.o: src/main.cpp
	g++ -c -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP -Isrc -I$INCLUDE_PATH -L$LIB_PATH -lraylib -o build/main.o src/main.cpp

ball.o: src/ball.cpp src/ball.h src/utils.h
	g++ -c -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP -Isrc -I$INCLUDE_PATH -L$LIB_PATH -lraylib -o build/ball.o src/ball.cpp

player.o: src/player.cpp src/player.h src/utils.h
	g++ -c -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP -Isrc -I$INCLUDE_PATH -L$LIB_PATH -lraylib -o build/player.o src/player.cpp

bot.o: src/bot.cpp src/bot.h
	g++ -c -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP -Isrc -I$INCLUDE_PATH -L$LIB_PATH -lraylib -o build/bot.o src/bot.cpp

utils.o: src/utils.cpp src/utils.h
	g++ -c -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP -Isrc -I$INCLUDE_PATH -L$LIB_PATH -lraylib -o build/utils.o src/utils.cpp

clean:
	rm -rf build
