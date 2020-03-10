LIBS_DIR = C:/libs
INCLUDE = $(LIBS_DIR)/include
LIB = $(LIBS_DIR)/lib
SRC = $(LIBS_DIR)/src

LIBS_PATH = -I$(INCLUDE) -L$(LIB)
LIBS = -lglfw3 -lopengl32 -lgdi32
LQ = litequarks

SOURCES = punyduck.cpp $(LQ)/LQuark.cpp $(LQ)/charges/LQGreenCharge.cpp\
		  LQTexture.cpp LQSurface.cpp LQWindow.cpp LQColor.cpp\
		  LQMoveCharge.cpp Shader.cpp $(SRC)/glad.c

all:
	g++ $(SOURCES) -o punyduck $(LIBS_PATH) $(LIBS) -Wfatal-errors
