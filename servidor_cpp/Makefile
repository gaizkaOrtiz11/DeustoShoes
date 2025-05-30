CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

SRC = src/Zapatilla.cpp src/Inventario.cpp src/Servidor.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
EXE = servidor

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXE)
