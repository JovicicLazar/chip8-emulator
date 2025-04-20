CXX = g++

FLAGS = -O3 -Wall -Wextra -lraylib

SRC = main.cpp $(wildcard src/*.cpp)

OBJ = $(SRC:%.cpp=%.o)

EXEC = play

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) $(FLAGS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXEC)