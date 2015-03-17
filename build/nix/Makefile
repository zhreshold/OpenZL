#

# A simple makefile for managing build of project composed of C source files.

#



# Source files to build

SOURCES = core.cpp highgui.cpp imgcodecs.cpp main.cpp

EXECUTABLE = demo



# In this section, you list the files that are part of the project.

# If you add/change names of source files, here is where you

# edit the Makefile.

SOURCE_DIR = ../../src/

OBJ_DIR = ./obj/

BIN_DIR = ../../bin/linux/

RM = rm -f





# It is likely that default C compiler is already gcc, but explicitly

# set, just to be sure

CC = g++



# The CFLAGS variable sets compile flags for gcc:

#  -g        compile with debug information

#  -Wall     give verbose compiler warnings

#  -O0       do not optimize generated code

#  -std=c99  use the C99 standard language definition

#  -m32      CS107 targets architecture IA32 (32-bit)

CFLAGS = -g -Wall -O0 -std=c++11 -I$(SOURCE_DIR)



# The LDFLAGS variable sets flags for linker

#  -lm   says to link in libm (the math library)

#  -lX11 X11 GUI library

LDFLAGS = -lX11 -pthread







#################################################

# !! DO NOT EDIT UNDER THIS

Objects=$(SOURCES:.cpp=.o)

CSources=$(addprefix $(SOURCE_DIR),$(SOURCES))

CObjects=$(addprefix $(OBJ_DIR),$(Objects))

CExecutable=$(addprefix $(BIN_DIR),$(EXECUTABLE))



all: $(CSources) $(CExecutable)



$(CExecutable): $(CObjects)
	mkdir -p $(BIN_DIR)

	$(CC) $(CObjects) $(LDFLAGS) -o $@



$(OBJ_DIR)%.o: $(SOURCE_DIR)%.cpp
	mkdir -p $(OBJ_DIR)

	$(CC) $(CFLAGS) -c -o $@ $<





# Phony means not a "real" target, it doesn't build anything

# The phony target "clean" is used to remove all compiled object files.



.PHONY: clean



clean:

	$(RM) $(CExecutable) $(CObjects)