/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:23:49 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/05 13:44:25 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

bool	tilemap_create(t_tilemap *map, uint16_t w, uint16_t h)
{
	if (__builtin_expect(!map || !w || !h, 0))
		return (false);
	map->w = w;
	map->h = h;
	map->area = w * h;
	map->spawn = (t_point){-1, -1};
	map->exit = (t_point){-1, -1};
	map->tiles = ft_calloc(sizeof(t_tile), map->area);
	return (map->tiles != NULL);
}

bool	tilemap_copy(t_tilemap *dst, t_tilemap *src)
{
	if (!tilemap_create(dst, src->w, src->h))
		return (false);
	ft_memcpy(dst->tiles, src->tiles, sizeof(t_tile) * src->area);
	dst->spawn = src->spawn;
	dst->exit = src->exit;
	return (true);
}

void	tilemap_destroy(t_tilemap *map)
{
	if (__builtin_expect(!map, 0))
		return ;
	free(map->tiles);
	ft_memset(map, 0, sizeof(t_tilemap));
}

t_tile	tilemap_get(t_tilemap *map, uint16_t i, uint16_t j)
{
	if (__builtin_expect(!map || !map->tiles || j >= map->w || i >= map->h, 0))
		return (FLOOR);
	return (*(map->tiles + i * map->w + j));
}

bool	tilemap_set(t_tilemap *map, uint16_t i, uint16_t j, t_tile t)
{
	if (__builtin_expect(!map || !map->tiles || j >= map->w || i >= map->h, 0))
		return (false);
	if (__builtin_expect(t != FLOOR && t != WALL && t != ITEM
			&& t != EXIT && t != SPAWN, 0))
		return (false);
	*(map->tiles + i * map->w + j) = t;
	return (true);
}
