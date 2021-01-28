###########################################
#     Declare some Makefile variables     #
###########################################
C_COMPILER = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
SRC_FILES = src/*.cpp \
			src/Logger/*.cpp \
			src/Game/*.cpp \
			src/ECS/*.cpp
INCLUDE_PATH = -IC:/MinGWLib/include -Iinclude
LIBRARY_PATH = -LC:/MinGWLib/lib -Llib
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua53
OBJ_NAME = engine

build:
	${C_COMPILER} ${LANG_STD} ${COMPILER_FLAGS} \
	${SRC_FILES} \
	${INCLUDE_PATH} \
	${LIBRARY_PATH} \
	${LINKER_FLAGS} \
	-o ${OBJ_NAME};

clean:
	rm ${OBJ_NAME};

run:
	./${OBJ_NAME};