# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/17 15:07:23 by ymeziane          #+#    #+#              #
#    Updated: 2024/02/01 15:40:21 by maxborde         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

# Files for each folder without .c extension
SRC_IS = ft_isalpha ft_isdigit ft_isalnum ft_isascii ft_isprint
SRC_STR = ft_strlen ft_strlcpy ft_strlcat ft_strchr ft_strrchr \
			ft_strnstr ft_strncmp ft_substr ft_strjoin ft_strtrim \
			ft_split ft_strmapi ft_striteri ft_strdup ft_strndup ft_strcmp
SRC_MEM = ft_memset ft_bzero ft_memcpy ft_memmove ft_memchr \
			ft_memcmp ft_calloc
SRC_TO = ft_atoi ft_toupper ft_tolower ft_itoa ft_atoll
SRC_PUT = ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_putnbr_fd
SRC_LST = ft_lstnew ft_lstadd_front ft_lstsize ft_lstlast \
			ft_lstadd_back ft_lstdelone ft_lstclear ft_lstiter \
			ft_lstmap
SRC_UTILS = ft_count_digit ft_hex_len

# Adding the .c extension back to the file names
SRC_IS := $(addsuffix .c, $(SRC_IS))
SRC_STR := $(addsuffix .c, $(SRC_STR))
SRC_MEM := $(addsuffix .c, $(SRC_MEM))
SRC_TO := $(addsuffix .c, $(SRC_TO))
SRC_PUT := $(addsuffix .c, $(SRC_PUT))
SRC_LST := $(addsuffix .c, $(SRC_LST))
SRC_UTILS := $(addsuffix .c, $(SRC_UTILS))

# Prefixing folder name to each file
SRCS_IS = $(addprefix $(SRC_DIR)/ft_is/, $(SRC_IS))
SRCS_STR = $(addprefix $(SRC_DIR)/ft_str/, $(SRC_STR))
SRCS_MEM = $(addprefix $(SRC_DIR)/ft_mem/, $(SRC_MEM))
SRCS_TO = $(addprefix $(SRC_DIR)/ft_to/, $(SRC_TO))
SRCS_PUT = $(addprefix $(SRC_DIR)/ft_put/, $(SRC_PUT))
SRCS_LST = $(addprefix $(SRC_DIR)/ft_lst/, $(SRC_LST))
SRCS_UTILS = $(addprefix $(SRC_DIR)/ft_utils/, $(SRC_UTILS))

# Merging all source files
SRCS = $(SRCS_IS) $(SRCS_STR) $(SRCS_MEM) $(SRCS_TO) $(SRCS_PUT) $(SRCS_LST) $(SRCS_UTILS)
OBJS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRCS:.c=.o))

RED = \033[0;31m
GREEN = \033[0;32m
NO_COLOR = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling libft...$(NO_COLOR)"
	@ar rcs $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "$(GREEN)Generating libft objects...$(NO_COLOR)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	@echo "$(RED)Cleaning libft objects...$(NO_COLOR)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)Deleting libft executable...$(NO_COLOR)"
	@rm -f $(NAME)

re: fclean $(NAME)
	@echo "$(GREEN)Recompiled libft$(NO_COLOR)"

.PHONY: all clean fclean re
