# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: emadriga <emadriga@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/11 15:36:58 by emadriga          #+#    #+#              #
#    Updated: 2022/08/10 22:28:45 by emadriga         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program's name
NAME	= philo

# compiling flags
FLAGS	=	-Wall -Wextra -Werror -Ofast $(SANITIZE)

# sanitize
# SANITIZE =
# SANITIZE = -g3 -fsanitize=address
SANITIZE = -g3 -fsanitize=thread

# Header files
INCLUDES_FILES =	philo.h

# Source and object files
SRC_FILES	= 	philo.c				\
				threads.c			\
				thread_life_cycle.c	\
				utils.c

OBJ_FILES	= $(SRC_FILES:.c=.o)

# Folders
SRC_DIR = ./src/
OBJ_DIR = ./obj/
INC_DIR = ./inc/

# Paths
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))
INCLUDES = $(addprefix $(INC_DIR), $(INCLUDES_FILES))

# all rule
all: obj $(NAME)

obj:
	@mkdir -p $(OBJ_DIR)
$(OBJ_DIR)%.o:$(SRC_DIR)%.c $(INCLUDES)
	@gcc $(FLAGS) -I $(INC_DIR) -o $@ -c $<
	
# Compiling
$(NAME): $(OBJ)
	@echo "\033[0;33mCompiling... Wait a sec.\033[0;31m"
	@gcc $(OBJ) $(FLAGS) -lm -o $(NAME)
	@echo "\033[0;32m$(NAME) generated!\033[0;37m"


# clean rule
clean:
			@rm -Rf $(OBJ_DIR)
			@echo "\033[1;34m[INFO]\033[0m Objects removed!"

# fclean rule
fclean:		clean
			@rm -f $(NAME)
			@echo "$(NAME) removed!"

# re rule
re:			fclean all

# phony
.PHONY:		all clean fclean re
