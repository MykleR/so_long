/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 19:11:29 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 22:38:54 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static bool find_exit(t_tilemap *map, uint16_t i, uint16_t j)
{
	t_tile	tile;

	if (__builtin_expect(!map || !map->tiles || j >= map->w || j >= map->h, 0))
		return (false);
	tile = tilemap_get(map, i, j);
	if (tile != FLOOR && tile != SPAWN)
	{
		if (tile == EXIT)
			map->exit = (t_point){i, j};
		return (tile == EXIT);
	}
	*(map->tiles + i * map->w + j) = 'V';
	return (find_exit(map, i + 1, j) || find_exit(map, i - 1, j)
		 || find_exit(map, i, j + 1) || find_exit(map, i, j - 1));
}

static bool	border_check(t_tilemap *map, uint16_t coord, bool axis)
{
	uint16_t	i;
	uint16_t	max;

	i = -1;
	max = map->h;
	if (axis)
		max = map->w;
	while (++i < max)
	{
		if (axis && tilemap_get(map, coord, i) != WALL)
			return (false);
		else if (!axis && tilemap_get(map, i, coord) != WALL)
			return (false);
	}
	return (true);
}

static void find_spawn(t_tilemap *map)
{
	uint16_t	i;
	uint16_t	j;

	i = -1;
	while (++i < map->h)
	{
		j = -1;
		while (++j < map->w)
			if (tilemap_get(map, i, j) == SPAWN)
				map->spawn = (t_point){i, j};
	}
}

bool	tilemap_check(t_tilemap *map)
{
	t_tilemap	cpy;
	bool		found_exit;
	
	if (!tilemap_copy(&cpy, map))
		return (false);
	if (!border_check(map, 0, 1) || !border_check(map, map->h - 1, 1)
		|| !border_check(map, 0, 0) || !border_check(map, map->w - 1, 0))
		return (false);
	find_spawn(map);
	found_exit = find_exit(&cpy, map->spawn.i, map->spawn.j);
	map->exit = cpy.exit;
	tilemap_destroy(&cpy);
	return (found_exit);
}
