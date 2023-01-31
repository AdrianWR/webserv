NAME = webserv

CXX      = c++
CXXFLAGS = -g -fsanitize=address
CPPFLAGS =
LDFLAGS  = -fsanitize=address

override CXXFLAGS += -Wall -Wextra -Werror -std=c++98
override CPPFLAGS += -Iinclude
override LDFLAGS	+=


OBJ_DIR = ./build
SRC_DIR = ./src

SRC   = $(notdir $(wildcard ./src/*.cpp))
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

.PHONY: intra run

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

intra:
	./webserv ./www/conf/conf_tester

run:
	./webserv ./www/conf/conf

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all