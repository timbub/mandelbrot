
CC = g++
DEBUG_FLAGS =  -O2 -fsanitize=address -g
RELEASE_FLAGS = -O3
TARGET = mandelbrot
OBJ = myprintf.o test_myprintf.o
FILE = mandelbrot.c
INTRINSICS = -mavx
SFML = -lsfml-graphics -lsfml-window -lsfml-system

release: clean
release: OPTIMIZATION = $(RELEASE_FLAGS)
release: $(TARGET)

debug: clean
debug: OPTIMIZATION = $(DEBUG_FLAGS)
debug: $(TARGET)

$(TARGET): $(FILE)
	$(CC)  $(FILE) $(OPTIMIZATION) $(INTRINSICS) $(SFML) -o $(TARGET)
clean:
	rm -f *.o $(TARGET)
