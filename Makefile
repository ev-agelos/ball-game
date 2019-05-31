game.exe: main.o ball.o player.o bot.o utils.o
	g++ -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP *.o -I. -IF:/raylib_64/include -LF:/raylib_64/lib -lraylib -o game.exe

main.o: main.cpp
	g++ -c -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP main.cpp -I. -IF:/raylib_64/include -LF:/raylib_64/lib -lraylib -o main.o

ball.o: ball.cpp ball.h utils.h
	g++ -c -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP ball.cpp -I. -IF:/raylib_64/include -LF:/raylib_64/lib -lraylib -o ball.o

player.o: player.cpp player.h utils.h
	g++ -c -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP player.cpp -I. -IF:/raylib_64/include -LF:/raylib_64/lib -lraylib -o player.o

bot.o: bot.cpp bot.h
	g++ -c -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP bot.cpp -I. -IF:/raylib_64/include -LF:/raylib_64/lib -lraylib -o bot.o

utils.o: utils.cpp utils.h
	g++ -c -g -std=c++17 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP utils.cpp -I. -IF:/raylib_64/include -LF:/raylib_64/lib -lraylib -o utils.o

clean:
	rm *.o game.exe