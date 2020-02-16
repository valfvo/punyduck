LIBS_DIR = C:/libs
INCLUDE = $(LIBS_DIR)/include
LIB = $(LIBS_DIR)/lib
SRC = $(LIBS_DIR)/src

LIBS_PATH = -I$(INCLUDE) -L$(LIB)
LIBS = -lglfw3 -lopengl32 -lgdi32

SOURCES = test007.cpp Shader.cpp litequarks/LQWindow.cpp\
		  litequarks/LQTexture.cpp $(SRC)/glad.c

all:
	g++ $(SOURCES) -o main $(LIBS_PATH) $(LIBS) -Wfatal-errors
