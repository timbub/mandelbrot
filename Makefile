
CC = g++
SANITIZE = -fsanitize=address -g
OPTIMIZATION = -O2
TARGET = mandelbrot
OBJ = myprintf.o test_myprintf.o
FILE = mandelbrot.c
INTRINSICS = -mavx
SFML = -lsfml-graphics -lsfml-window -lsfml-system
all: $(TARGET)
$(TARGET): $(FILE)
	$(CC)  $(FILE) -o $(OPTIMIZATION) $(SANITIZE) $(INTRINSICS) $(SFML) -o $(TARGET)
