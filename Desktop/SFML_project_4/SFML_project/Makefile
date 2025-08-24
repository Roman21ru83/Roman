# Názvy spustiteľných súborov
MAIN_EXEC = main-app
START_EXEC = start

# Kompilátor a flagy
CXX = g++
CXXFLAGS = -std=c++17 -Wall
SFML = -lsfml-graphics -lsfml-window -lsfml-system

# Súčasti
all: $(START_EXEC) $(MAIN_EXEC)

$(START_EXEC): start.cpp
	$(CXX) $(CXXFLAGS) start.cpp -o $(START_EXEC) $(SFML)

$(MAIN_EXEC): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(MAIN_EXEC) $(SFML)

clean:
	rm -f $(START_EXEC) $(MAIN_EXEC)
