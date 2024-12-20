# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrouves <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/08 18:27:35 by mrouves           #+#    #+#              #
#    Updated: 2024/12/20 20:26:00 by mrouves          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq (bonus,$(MAKECMDGOALS))
include sources/sources_bonus.mk
else
include sources/sources_manda.mk
endif

include sources/pretty_compile.mk

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
CFLAGS			:= -Wall -Wextra -Werror -O3
IFLAGS			:= -I $(DIR_HEADERS) -I $(MLX_INCLUDES) -I $(LIBFT_INCLUDES) -I $(ECS_INCLUDES)
DIR_DUP			= mkdir -p $(@D)

all: $(NAME) $(OBJS)

bonus: all

$(NAME): $(OBJS) $(ECS) $(MLX) $(LIBFT)
	@$(call run_and_test, $(CC) $(CFLAGS) $(IFLAGS) $^ -o $@ -lm -lSDL2)
	@printf "$(BOLD)$(NAME)$(NO_COLOR) compiled $(OK_COLOR)successfully$(NO_COLOR)\n"

$(ECS):
	@$(MAKE) -C $(DIR_ECS) --no-print-directory -j

$(LIBFT):
	@$(MAKE) -C $(DIR_LIBFT) --no-print-directory -j

$(MLX):
	@$(MAKE) -C $(DIR_MLX) --no-print-directory -j)

$(DIR_OBJS)/%.o: $(DIR_SOURCES)/%.c
	@$(DIR_DUP)
	@$(call run_and_test,$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@)

clean:
	@rm -rf $(DIR_OBJS)
	@printf "Cleaned $(BOLD)$(DIR_OBJS)$(NO_COLOR)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "Cleaned $(BOLD)$(NAME)$(NO_COLOR)\n"
	@$(MAKE) -C $(DIR_ECS) --no-print-directory fclean
	@$(MAKE) -C $(DIR_LIBFT) --no-print-directory fclean
#	@$(MAKE) -C $(DIR_MLX) --no-print-directory fclean	

re: fclean all

.PHONY: clean fclean re bonus all
