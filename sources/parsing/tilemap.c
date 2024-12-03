/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:23:49 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 19:18:30 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	tilemap_create(t_tilemap *map, uint32_t w, uint32_t h)
{
	if (__builtin_expect(!map || !w || !h, 0))
		return (false);
	map->w = w;
	map->h = h;
	map->area = w * h;
	map->tiles = ft_calloc(sizeof(t_tile), map->area);
	return (map->tiles != NULL);
}

void	tilemap_destroy(t_tilemap *map)
{
	if (__builtin_expect(!map, 0))
		return ;
	free(map->tiles);
	ft_memset(map, 0, sizeof(t_tilemap));
}

t_tile	tilemap_get(t_tilemap *map, uint32_t x, uint32_t y)
{
	if (__builtin_expect(!map || !map->tiles || x >= map->w || y >= map->h, 0))
		return (FLOOR);
	return (*(map->tiles + y * map->w + x));
}

void	tilemap_set(t_tilemap *map, uint32_t x, uint32_t y, t_tile t)
{
	if (__builtin_expect(!map || !map->tiles || x >= map->w || y >= map->h, 0))
		return ;
	if (__builtin_expect(t != FLOOR && t != WALL && t != ITEM
		&& t != EXIT && t != SPAWN, 0))
		t = FLOOR;
	*(map->tiles + y * map->w + x) = t;
}

