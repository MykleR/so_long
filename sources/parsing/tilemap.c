/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:23:49 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/05 20:27:55 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

bool	tilemap_create(t_tilemap *map, uint8_t w, uint8_t h)
{
	if (__builtin_expect(!map || !w || !h || w >= TILEMAP_MAX_SIZE
			|| h >= TILEMAP_MAX_SIZE, 0))
		return (false);
	map->size = (t_point){h, w};
	map->area = w * h;
	map->spawn = (t_point){-1, -1};
	map->exit = (t_point){-1, -1};
	map->to_collect = 0;
	map->tiles = ft_calloc(sizeof(t_tile), map->area);
	return (map->tiles != NULL);
}

bool	tilemap_copy(t_tilemap *dst, t_tilemap src)
{
	if (!tilemap_create(dst, src.size.j, src.size.i))
		return (false);
	ft_memcpy(dst->tiles, src.tiles, sizeof(t_tile) * src.area);
	dst->spawn = src.spawn;
	dst->exit = src.exit;
	dst->to_collect = src.to_collect;
	return (true);
}

void	tilemap_destroy(t_tilemap *map)
{
	if (__builtin_expect(!map, 0))
		return ;
	free(map->tiles);
	ft_memset(map, 0, sizeof(t_tilemap));
}

t_tile	tilemap_get(t_tilemap map, uint8_t i, uint8_t j)
{
	if (__builtin_expect(!map.tiles || j >= map.size.j || i >= map.size.i, 0))
		return (FLOOR);
	return (*(map.tiles + i * map.size.j + j));
}

bool	tilemap_set(t_tilemap *map, uint8_t i, uint8_t j, t_tile t)
{
	if (__builtin_expect(!map || !map->tiles
			|| j >= map->size.j || i >= map->size.i, 0))
		return (false);
	if (__builtin_expect(t != FLOOR && t != WALL && t != ITEM
			&& t != EXIT && t != SPAWN, 0))
		return (false);
	*(map->tiles + i * map->size.j + j) = t;
	return (true);
}
