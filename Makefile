NAME = webserv

CXX      = c++
CXXFLAGS =
CPPFLAGS =
LDFLAGS  =

override CXXFLAGS += -Wall -Wextra -Werror -std=c++98
override CPPFLAGS += -Iinclude
override LDFLAGS	+=


OBJ_DIR = ./build
SRC_DIR = ./src

SRC_FILES   = $(wildcard $(SRC_DIR)/*.cpp)
INCLUDE_FILES = $(wildcard include/*.hpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

.PHONY: intra run

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp $(INCLUDE_FILES)
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

intra: $(NAME)
	./$< ./www/conf/conf_tester

run: $(NAME)
	./$< ./www/conf/conf

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all