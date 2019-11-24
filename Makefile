CXX = clang++
CXXFLAGS = -Wall -c -std=c++11
LDFLAGS = -F/Library/Frameworks -framework SDL2
EXE = learning-sdl

all: $(EXE)

$(EXE): main.o
	$(CXX) $(LDFLAGS) $< -o $@

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run: $(EXE)
	./$(EXE)

clean:
	rm *.o && rm $(EXE)
