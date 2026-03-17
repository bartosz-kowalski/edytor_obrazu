CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -Iraylib/include -Izaleznosci -DTEXTURES_DIR=\"$(CURDIR)/tekstury\"
LDFLAGS = -Lraylib/lib -lraylib -lX11 -lXrandr -lXinerama -lXi -lXcursor -lGL -lrt -lpthread -ldl -lm

SRC_MAIN = edytor_obrazu.cpp
SRC_DEPS = $(wildcard zaleznosci/*.cpp)

OBJ = $(SRC_MAIN:.cpp=.o) $(SRC_DEPS:.cpp=.o)

TARGET = edytor_obrazu

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)