# Declaration of variables
CC = g++
CFLAGS = -fPIC -I. `sdl2-config --cflags` -c
LFLAGS = -L/usr/lib `sdl2-config --libs` -lpthread

# File names
EXEC = project_vulture.bin
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LFLAGS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
