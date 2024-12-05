/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 19:11:29 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/05 13:51:04 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static uint16_t exit_check(t_tilemap *map, uint16_t i, uint16_t j, bool *found)
{
	t_tile		tile;

	if (__builtin_expect(!map || !map->tiles || j >= map->w || j >= map->h, 0))
		return (0);
	tile = tilemap_get(map, i, j);
	if (tile == EXIT)
	{
		*found = (map->exit.i == UINT16_MAX);
		map->exit = (t_point){i, j};
	}
	if (tile != FLOOR && tile != SPAWN && tile != EXIT && tile != ITEM)
		return (0);
	*(map->tiles + i * map->w + j) = 'V';
	return (exit_check(map, i + 1, j, found) + exit_check(map, i - 1, j, found)
		+ exit_check(map, i, j + 1, found) + exit_check(map, i, j -  1, found)
		+ (tile == ITEM));
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
		if ((axis && tilemap_get(map, coord, i) != WALL)
			|| (!axis && tilemap_get(map, i, coord) != WALL))
			return (false);
	}
	return (true);
}

static bool spawn_check(t_tilemap *map, uint16_t *nb_items)
{
	t_tile		tile;
	uint16_t	i;
	uint16_t	j;
	uint16_t	nb_spawns;

	*nb_items = 0;
	nb_spawns = 0;
	i = -1;
	while (++i < map->h)
	{
		j = -1;
		while (++j < map->w)
		{
			tile = tilemap_get(map, i, j);
			(*nb_items) += tile == ITEM;
			nb_spawns += tile == SPAWN;
			if (tile == SPAWN)
				map->spawn = (t_point){i, j};
		}
	}
	return (nb_spawns == 1);
}

t_parse_error	tilemap_check(t_tilemap *map)
{
	t_tilemap	cpy;
	uint16_t	nb_items;
	bool		found_exit;

	found_exit = false;
	if (!tilemap_copy(&cpy, map))
		return (PARSE_ERROR_MEMCRASH);
	if (!border_check(map, 0, 1) || !border_check(map, map->h - 1, 1)
		|| !border_check(map, 0, 0) || !border_check(map, map->w - 1, 0))
		return (PARSE_ERROR_NOBOUNDS);
	if( !spawn_check(map, &nb_items))
		return (PARSE_ERROR_BADSPAWN);
	if (!nb_items)
		return (PARSE_ERROR_NOITEM);
	if (exit_check(&cpy, map->spawn.i, map->spawn.j, &found_exit) != nb_items
		|| !found_exit)
		return (PARSE_ERROR_BADEXIT);
	map->exit = cpy.exit;
	tilemap_destroy(&cpy);
	return (PARSE_OK);
}
