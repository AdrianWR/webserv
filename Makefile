NAME = webserv

FLAGS = -Wall -Wextra -Werror -std=c++98
FLAGS += -g
FLAGS += -fsanitize=address

CC = c++
# CC = clang++

OBJ_PATH = ./obj
SRC_PATH = ./src

LIB_PATH = ./lib
LIB_NAME = webserv.a

LIB = $(LIB_PATH)/$(LIB_NAME)

SRC = $(notdir $(wildcard ./src/*.cpp))
OBJ=$(addprefix $(OBJ_PATH)/, $(SRC:.cpp=.o))

all: pre $(NAME)

clean:
	rm -fR ./obj
	rm -fR ./lib

fclean: clean
	sudo rm /etc/hosts
	sudo cp ./hosts_backup /etc/hosts

	rm -f $(NAME)

re: fclean all

pre:

	mkdir -p obj
	mkdir -p lib
	cp /etc/hosts ./hosts_backup
	sudo rm /etc/hosts
	sudo cp ./config_hosts /etc/hosts


$(NAME): $(LIB)
	$(CC) $(FLAGS) -g  main.cpp  -L. -I ./src $(LIB) -o $@

$(LIB): $(OBJ)
	ar rcs $@ $(OBJ)
	ranlib $@

$(OBJ_PATH)/%.o:	$(SRC_PATH)/%.cpp
	$(CC) -g $(FLAGS) -c $< -o $@

.PHONY: intra run pre

intra:
	./webserv ./www/conf/conf_tester
run:
	./webserv ./www/conf/conf

