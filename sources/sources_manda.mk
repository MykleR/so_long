# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources_manda.mk                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrouves <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/29 16:59:36 by mrouves           #+#    #+#              #
#    Updated: 2024/12/03 23:09:50 by mrouves          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
				collisions/collision_check.c\
				collisions/collision_grid.c\
				array_list.c\

