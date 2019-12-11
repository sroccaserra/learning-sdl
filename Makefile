CXX = clang++
CXXFLAGS = -Wall -c -std=c++17
LDFLAGS = -lsdl2
EXE = learning-sdl

all: $(EXE)

$(EXE): main.o
	$(CXX) $(LDFLAGS) $< -o $@

main.o: src/main.cpp src/Screen.h
	$(CXX) $(CXXFLAGS) $< -o $@

run: $(EXE)
	./$(EXE)

clean:
	rm -f *.o && rm -f $(EXE)
