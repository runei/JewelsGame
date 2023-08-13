CXX = g++
CXXFLAGS = -std=c++17
LIBS = -lSDL2 -lSDL2_image

SRC_DIR = src
COMMON_DIR = $(SRC_DIR)/common
MODEL_DIR = $(SRC_DIR)/model
VIEW_DIR = $(SRC_DIR)/view
VIEWMODEL_DIR = $(SRC_DIR)/viewmodel

SRCS = $(wildcard $(SRC_DIR)/*.cpp $(COMMON_DIR)/*.cpp $(MODEL_DIR)/*.cpp $(VIEW_DIR)/*.cpp  $(VIEWMODEL_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)

EXECUTABLE = jewelsgame

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(EXECUTABLE)
