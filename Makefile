#
#	Makefile Template
#	SDL2, GLEW, GLM, SDL_TTF
#

SRCPATH := ./src
INCPATH := ./include
OBJPATH := ./obj
BINPATH := ./bin

SRCS := $(wildcard $(SRCPATH)/*.cpp)
OBJS := $(patsubst $(SRCPATH)/%.cpp, $(OBJPATH)/%.o, $(SRCS))

CC := g++
LIBRARIES := -lSDL2 -lGLEW -lGL -lSDL2_ttf
WARNING := -Werror
CPPFLAGS := -std=c++17 -I $(INCPATH)/ -O2

RESULT := Gewirrenth

.PHONY: build run clean

build: $(OBJS) | $(BINPATH)
	$(CC) $(LIBRARIES) -o $(BINPATH)/$(RESULT) $(OBJS)

run:
	$(MAKE) -C bin

clean:
	rm -rf $(OBJPATH)/*
	rm -f $(BINPATH)/$(RESULT)

$(OBJPATH)/%.o : $(SRCPATH)/%.cpp | $(OBJPATH)
	$(CC) $(CPPFLAGS) $(WARNING) $(LIBRARIES) -c -o $@ $<
	
$(OBJPATH):
	mkdir $(OBJPATH)
	
$(BINPATH):
	mkdir $(BINPATH)
