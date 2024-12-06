# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources_manda.mk                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrouves <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/29 16:59:36 by mrouves           #+#    #+#              #
#    Updated: 2024/12/06 16:04:18 by mrouves          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
define TITLE

┌────────────────────────────┐
│░█▀▀░█▀█░░░░░█░░░█▀█░█▀█░█▀▀│
│░▀▀█░█░█░░░░░█░░░█░█░█░█░█░█│
│░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░▀░▀▀▀│
└────────────────────────────┘
           MANDA
endef
export TITLE
text-style 		= $(shell tput $1)$2$(shell tput sgr0)
PRINT_TITLE		:= $(shell $(info $(call text-style,setaf 6, $(TITLE))))

SOURCES		:=	manda/main.c\
				manda/on_init.c\
				manda/on_event.c\
				manda/on_update.c\
				manda/on_clear.c\
				application/app.c\
				application/app_hooks.c\
				application/app_extra.c\
				parsing/tilemap.c\
				parsing/tilemap_parsing.c\
				parsing/tilemap_check.c\
				collisions/collision_check.c\
				collisions/collision_grid.c\

