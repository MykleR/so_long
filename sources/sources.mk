# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources_manda.mk                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrouves <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/29 16:59:36 by mrouves           #+#    #+#              #
#    Updated: 2024/12/22 12:58:05 by mrouves          ###   ########.fr        #
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
				parsing/tilemap_check.c\
				collisions/collision_check.c\
				collisions/collision_grid.c\

SOURCES_BONUS	:=	bonus/main.c\
				bonus/resources.c\
				bonus/menus/menu_start.c\
				bonus/menus/menu_death.c\
				bonus/game/systems_render.c\
				bonus/game/systems_physics.c\
				bonus/game/particles.c\
				bonus/game/player.c\
				bonus/game/enemy.c\
				bonus/game/game.c\
				bonus/game/game_event.c\
				application/app.c\
				application/app_hooks.c\
				application/app_extra.c\
				parsing/tilemap.c\
				parsing/tilemap_parsing.c\
				parsing/tilemap_check.c\
				collisions/collision_check.c\
				collisions/collision_grid.c\

