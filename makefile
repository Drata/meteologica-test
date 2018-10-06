# Compiler
CC = gcc

# Flags
CFLAGS  = -ansi -std=c99 -g -Wall
CFLAGS_GUI += -std=c99 -g -pedantic -O2

# Libraries
GLFW3 := $(shell pkg-config --libs glfw3)
LIBS = $(GLFW3) -lGL -lm -lGLU -lGLEW


meteologica: meteologica_console meteologica_gui clean

meteologica_gui: main_gui.o weather_info.o jWrite.o csvparser.o str_util.o
	rm -f bin/meteologica_gui
	$(CC) $(CFLAGS_GUI) -o bin/meteologica_gui main_gui.o weather_info.o jWrite.o csvparser.o str_util.o $(LIBS)

meteologica_console:  main_console.o weather_info.o jWrite.o csvparser.o str_util.o
	rm -f bin/meteologica_console
	$(CC) $(CFLAGS) -o bin/meteologica_console main_console.o weather_info.o jWrite.o csvparser.o str_util.o

main_gui.o: src/main_gui.c include/weather_info.h include/csvparser.h
	$(CC) $(CFLAGS_GUI) -c src/main_gui.c

main_console.o:  src/main_console.c include/weather_info.h include/csvparser.h
	$(CC) $(CFLAGS) -c src/main_console.c

weather_info.o:  src/weather_info.c include/weather_info.h include/uthash.h include/jWrite.h include/str_util.h
	$(CC) $(CFLAGS) -c src/weather_info.c

jWrite.o: src/jWrite.c include/jWrite.h
	$(CC) $(CFLAGS) -c src/jWrite.c

csvparser.o: src/csvparser.c include/csvparser.h
	$(CC) $(CFLAGS) -c src/csvparser.c

str_util.o: src/str_util.c include/str_util.h
	$(CC) $(CFLAGS) -c src/str_util.c

clean:
	$(RM) count *.o *~
