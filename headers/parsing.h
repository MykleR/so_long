/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 17:57:36 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 19:19:51 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "libft.h"
# include <fcntl.h>

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
	uint64_t	area;
	uint32_t	w;
	uint32_t	h;
}	t_tilemap;

bool	tilemap_parse(t_tilemap *map, const char *path);
bool	tilemap_create(t_tilemap *map, uint32_t w, uint32_t h);
void	tilemap_destroy(t_tilemap *map);
t_tile	tilemap_get(t_tilemap *map, uint32_t x, uint32_t y);
void	tilemap_set(t_tilemap *map, uint32_t x, uint32_t y, t_tile t);

#endif
