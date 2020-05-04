CXX = g++
# dynamic compilation
# CXXFLAGS = -Wall -Wfatal-errors -MMD
# static compilation
CXXFLAGS = -Wall -Wfatal-errors -MMD -static -mwindows

LIBS_DIR = C:/Users/valfv/libs
INCLUDE_DIR = $(LIBS_DIR)/include
LIB_DIR = $(LIBS_DIR)/lib
SRC_DIR = $(LIBS_DIR)/src

# replace the path below with your python38 path
PYTHON_PATH = C:/Users/valfv/AppData/Local/Programs/Python/Python38
PYTHON_INCLUDE = $(PYTHON_PATH)/include
PYTHON_LIBS = $(PYTHON_PATH)/libs

LQ_DIR = ../litequarks
LQ_SRC := $(wildcard $(LQ_DIR)/src/*.cpp)
LQ_OBJ_PREFIXLESS := $(patsubst src/%.cpp,obj/%.o,$(LQ_SRC:$(LQ_DIR)/%=%))
LQ_OBJ := $(addprefix $(LQ_DIR)/,$(LQ_OBJ_PREFIXLESS))

PN_SRC := $(wildcard *.cpp) $(wildcard client/*.cpp)
PN_OBJ := $(PN_SRC:.cpp=.o)

LIBS_PATH = -I$(INCLUDE_DIR) -I$(PYTHON_INCLUDE) -I$(LQ_DIR)/include -I. \
	-L$(LIB_DIR) -L$(PYTHON_LIBS)

LIBS = -lglfw3 -lopengl32 -lgdi32 -lfreetype -lpython38
LIBS_FLAGS = $(LIBS_PATH) $(LIBS)

OBJ = $(PN_OBJ) $(LQ_OBJ) $(SRC_DIR)/glad.o

all: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o main $(LIBS_FLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS_FLAGS)

client/%.o: client/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS_FLAGS)

$(LQ_DIR)/obj/%.o: $(LQ_DIR)/src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS_FLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS_FLAGS)

-include $(OBJ:.o=.d)
