# Copyright 2025 <Orbosa Igbinovia> [legal/copyright]

# Compiler and flags
CC = g++
CFLAGS = -std=c++20 -Wall -Werror -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework

# Source files
SRC = main.cpp Sokoban.cpp
DEPS = Sokoban.hpp
OBJ = $(SRC:.cpp=.o)

# Test file
TEST_SRC = test.cpp

# Targets
PROGRAM = Sokoban
STATIC_LIB = Sokoban.a

# Phony targets
.PHONY: all clean lint

# Default target (Builds everything)
all: $(PROGRAM) $(STATIC_LIB)

# Rule for linking the main program
$(PROGRAM): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

# Rule for linking and building the test program
test: test.o Sokoban.o
	$(CC) $(CFLAGS) -o test test.o Sokoban.o $(LIB)

# Rule for compiling object files
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for creating the static library (excluding main.o)
$(STATIC_LIB): $(filter-out main.o, $(OBJ))
	ar rcs $@ $^

# Clean target
clean:
	rm -f $(OBJ) test.o test $(PROGRAM) $(STATIC_LIB)

# Linting
lint:
	cpplint --filter=-whitespace $(SRC) $(DEPS)
