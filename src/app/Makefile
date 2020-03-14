CXX = g++
CXXFLAGS = -Wall -Wfatal-errors -MMD

LIBS_DIR = C:/Users/valfv/libs
INCLUDE_DIR = $(LIBS_DIR)/include
LIB_DIR = $(LIBS_DIR)/lib
SRC_DIR = $(LIBS_DIR)/src

LQ_DIR = ../litequarks
LQ_SRC := $(wildcard $(LQ_DIR)/src/*.cpp)
LQ_OBJ_PREFIXLESS := $(patsubst src/%.cpp,obj/%.o,$(LQ_SRC:$(LQ_DIR)/%=%))
LQ_OBJ := $(addprefix $(LQ_DIR)/,$(LQ_OBJ_PREFIXLESS))

LIBS_PATH = -I$(INCLUDE_DIR) -I$(LQ_DIR)/include -L$(LIB_DIR)
LIBS = -lglfw3 -lopengl32 -lgdi32
LIBS_FLAGS = $(LIBS_PATH) $(LIBS)

OBJ = main.o $(LQ_OBJ) $(SRC_DIR)/glad.o

all: $(OBJ)
	$(CXX) $^ -o main $(LIBS_FLAGS)

main.o: main.cpp
	g++ $(CXXFLAGS) -c $< -o $@ $(LIBS_FLAGS)

$(LQ_DIR)/obj/%.o: $(LQ_DIR)/src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS_FLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS_FLAGS)

-include $(SRC_DIR:.o=.d)
-include $(LQ_DIR/obj:.o=.d)
