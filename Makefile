CXX = g++
CXXFLAGS = -std=c++17
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lpthread -ldl

SRC_DIR = src
COMMON_DIR = $(SRC_DIR)/common
MODEL_DIR = $(SRC_DIR)/model
VIEW_DIR = $(SRC_DIR)/view
VIEWMODEL_DIR = $(SRC_DIR)/viewmodel
EXTERNAL_DIR = $(SRC_DIR)/external
COMPONENTS_DIR = $(SRC_DIR)/components

BUILD_DIR = build
LINUX_EXECUTABLE = $(BUILD_DIR)/jewelsgame_linux
WINDOWS_EXECUTABLE = $(BUILD_DIR)/jewelsgame.exe
MACOS_EXECUTABLE = $(BUILD_DIR)/jewelsgame_macos

SRCS = $(wildcard $(SRC_DIR)/*.cpp $(COMMON_DIR)/*.cpp $(MODEL_DIR)/*.cpp $(VIEW_DIR)/*.cpp  $(VIEWMODEL_DIR)/*.cpp $(EXTERNAL_DIR)/*.cpp $(COMPONENTS_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all: linux windows macos

linux: $(LINUX_EXECUTABLE)

windows: $(WINDOWS_EXECUTABLE)

macos: $(MACOS_EXECUTABLE)

$(LINUX_EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(LINUX_EXECUTABLE) $(OBJS) $(LIBS)

$(WINDOWS_EXECUTABLE): $(OBJS)
	x86_64-w64-mingw32-g++ $(CXXFLAGS) -o $(WINDOWS_EXECUTABLE) $(OBJS) $(LIBS)

$(MACOS_EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MACOS_EXECUTABLE) $(OBJS) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(LINUX_EXECUTABLE) $(WINDOWS_EXECUTABLE) $(MACOS_EXECUTABLE)
