
CC = g++
# SANITIZE = -fsanitize=address -g //TODO in release, add all flags release_build, dev_build,
OPTIMIZATION = -O0 #TODO use -O0 and -O3
TARGET = mandelbrot
OBJ = myprintf.o test_myprintf.o #TODO sfml - auto
FILE = mandelbrot.c
INTRINSICS = -mavx
SFML = -lsfml-graphics -lsfml-window -lsfml-system #TODO add clean
all: $(TARGET)
$(TARGET): $(FILE)
	$(CC)  $(FILE) $(OPTIMIZATION) $(INTRINSICS) $(SFML) -o $(TARGET) #TODO remove first -o
