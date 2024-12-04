/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 17:57:36 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 15:16:56 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <libft.h>
# include <fcntl.h>

# define TILE_SIZE 32

typedef enum e_tile
{
	FLOOR = '0',
	WALL = '1',
	ITEM = 'C',
	EXIT = 'E',
	SPAWN = 'P'
}	t_tile;

typedef struct s_tilemap
{
	t_tile		*tiles;
	uint32_t	area;
	uint16_t	w;
	uint16_t	h;
	uint16_t	spawn_x;
	uint16_t	spawn_y;
	uint16_t	exit_x;
	uint16_t	exit_y;
}	t_tilemap;

bool	tilemap_parse(t_tilemap *map, const char *path);
bool	tilemap_create(t_tilemap *map, uint16_t w, uint16_t h);
void	tilemap_destroy(t_tilemap *map);
t_tile	tilemap_get(t_tilemap *map, uint16_t x, uint16_t y);
void	tilemap_set(t_tilemap *map, uint16_t x, uint16_t y, t_tile t);

#endif
