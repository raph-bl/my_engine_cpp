CC                      = g++
CPPFLAGS                = -std=c++17 -Wall -I. -Iinclude
PKG_CFLAGS              := $(shell pkg-config --cflags glfw3 2> /dev/null)
PKG_LIBS                := $(shell pkg-config --libs glfw3 2> /dev/null)
LDFLAGS                 = $(PKG_LIBS) -lGLEW -lGL -lGLU -lglut

SRC                     := $(shell find src -name '*.cpp')
OBJ                     := $(SRC:.cpp=.o)
NAME                    := my_engine

all: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

src/%.o: src/%.cpp
	$(CC) $(CPPFLAGS) $(PKG_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(NAME)

re: clean all

distclean: clean
	@echo "distclean: nothing extra to remove"

.PHONY: all clean re distclean