CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = src/main.cpp \
      src/menu.cpp \
      src/visuals.cpp \
      src/crypto.cpp \
      src/fileutils.cpp \
      src/metadata.cpp

OBJ = $(SRC:.cpp=.o)

BIN = UmbraCrypt

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(OBJ)

clean:
	rm -f $(OBJ) $(BIN)

