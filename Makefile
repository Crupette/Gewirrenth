#
#	Makefile Template
#	SDL2, GLEW, GLM
#	

SRCPATH := ./src
INCPATH := ./include
OBJPATH := ./obj
BINPATH := ./bin

SRCS := $(wildcard $(SRCPATH)/*.cpp)
OBJS := $(patsubst $(SRCPATH)/%.cpp, $(OBJPATH)/%.o, $(SRCS))

CC := g++
LIBRARIES := -lSDL2 -lGLEW -lGL
WARNING := -Werror
CPPFLAGS := -std=c++17 -I $(INCPATH)/

RESULT := Sidescroller

.PHONY: build run clean

build: $(OBJS)
	$(CC) $(LIBRARIES) -o $(BINPATH)/$(RESULT) $(OBJS)
	
run:
	$(BINPATH)/$(RESULT)
	
clean::
	rm -rf $(OBJPATH)/*
	rm -f $(BINPATH)/$(RESULT)

$(OBJPATH)/%.o : $(SRCPATH)/%.cpp
	$(CC) $(CPPFLAGS) $(WARNING) $(LIBRARIES) -c -o $@ $<
	
