# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources_bonus.mk                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/03 17:46:12 by mrouves           #+#    #+#              #
#    Updated: 2024/12/06 16:04:13 by mrouves          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
define TITLE

┌────────────────────────────┐
│░█▀▀░█▀█░░░░░█░░░█▀█░█▀█░█▀▀│
│░▀▀█░█░█░░░░░█░░░█░█░█░█░█░█│
│░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░▀░▀▀▀│
└────────────────────────────┘
           BONUS
endef
export TITLE
text-style 		= $(shell tput $1)$2$(shell tput sgr0)
PRINT_TITLE		:= $(shell $(info $(call text-style,setaf 6, $(TITLE))))

SOURCES		:=	bonus/main.c\
				bonus/menu/on_init.c\
				bonus/menu/on_event.c\
				bonus/menu/on_update.c\
				bonus/menu/on_clear.c\
				bonus/game/on_init.c\
				bonus/game/on_event.c\
				bonus/game/on_update.c\
				bonus/game/on_clear.c\
				application/app.c\
				application/app_hooks.c\
				application/app_extra.c\
				parsing/tilemap.c\
				parsing/tilemap_parsing.c\
				parsing/tilemap_check.c\
				collisions/collision_check.c\
				collisions/collision_grid.c\
