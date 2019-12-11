CXX = clang++
CXXFLAGS = -Wall -std=c++17 -Iinclude
LDFLAGS = -lsdl2

CPP_FILES = $(wildcard src/*.cpp)
HEADERS_FILES = $(wildcard src/*.h)
OBJ_FILES = $(CPP_FILES:.cpp=.o)

EXEC_NAME = learning-sdl

all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $(OBJ_FILES) -o $@

run: $(EXEC_NAME)
	./$(EXEC_NAME)

clean:
	rm -f $(OBJ_FILES)
	rm -f $(EXEC_NAME)
