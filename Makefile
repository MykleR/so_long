# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrouves <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/08 18:27:35 by mrouves           #+#    #+#              #
#    Updated: 2024/12/03 23:43:54 by mrouves          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(MODE), bonus)
include sources/sources_bonus.mk
else
include sources/sources_manda.mk
endif

NAME 			:= so_long
DIR_HEADERS		:= headers
DIR_SOURCES		:= sources
DIR_OBJS		:= .objs
DIR_LIB			:= lib

DIR_MLX			:= $(DIR_LIB)/MacroLibX
MLX_INCLUDES	:= $(DIR_MLX)/includes
MLX				:= $(DIR_MLX)/libmlx.so

DIR_LIBFT		:= $(DIR_LIB)/libft
LIBFT_INCLUDES	:= $(DIR_LIBFT)/headers
LIBFT			:= $(DIR_LIBFT)/libft.a

DIR_ECS			:= $(DIR_LIB)/lib-ecs
ECS_INCLUDES	:= $(DIR_ECS)/headers
ECS				:= $(DIR_ECS)/lib-ecs.a

OBJS			:= $(addprefix $(DIR_OBJS)/, $(SOURCES:%.c=%.o))

CC				:= clang
CFLAGS			:= -Wall -Wextra -Werror -O3 -DNDEBUG -g
IFLAGS			:= -I $(DIR_HEADERS) -I $(MLX_INCLUDES) -I $(LIBFT_INCLUDES) -I $(ECS_INCLUDES)

GREEN			= \033[0;32m
RED				= \033[0;31m
END				= \033[0m
DIR_DUP			= mkdir -p $(@D)

all: $(NAME) $(OBJS)

bonus: 
	@$(MAKE) MODE=$@ --no-print-directory -j

$(NAME): $(OBJS) $(ECS) $(MLX) $(LIBFT)
	@$(CC) $(CFLAGS) $(IFLAGS) $^ -o $@ -lm -lSDL2
	@printf "$(GREEN)$(NAME) compiled$(END)\n"

$(ECS):
	@$(MAKE) -C $(DIR_ECS) --no-print-directory -j

$(LIBFT):
	@$(MAKE) -C $(DIR_LIBFT) --no-print-directory -j

$(MLX):
	@$(MAKE) -C $(DIR_MLX) --no-print-directory -j

$(DIR_OBJS)/%.o: $(DIR_SOURCES)/%.c
	@$(DIR_DUP)
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	@rm -rf $(DIR_OBJS)
	@printf "$(RED)$(NAME) cleaned objs$(END)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(RED)$(NAME) removed$(END)\n"
	@$(MAKE) -C $(DIR_ECS) --no-print-directory fclean
	@$(MAKE) -C $(DIR_LIBFT) --no-print-directory fclean
	@$(MAKE) -C $(DIR_MLX) --no-print-directory fclean

re: fclean all

.PHONY: clean fclean re all
