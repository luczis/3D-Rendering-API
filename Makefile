#
# Raster Project
# 
# 

# Object file list
OBJS = main.o

# Executable Name
EXENAME = Raster

# Macros
CC = g++
CCOPTS = -c -g -O0 -Wall -Wno-deprecated
LINK = g++
LINKOPTS = -o

all : $(EXENAME)

# OS Linux
OS = $(shell uname -s)
LIBS = -lGL -lGLU -lglut

# LINK OBJECT FILES
$(EXENAME) : $(OBJS)
	$(LINK) $(LINKOPTS) $(EXENAME) $(OBJS) $(LIBS)

# COMPILE OBJECT FILES
main.o : main.cpp
	$(CC) $(CCOPTS) main.cpp 

clean :
	rm -f *.o $(EXENAME)
