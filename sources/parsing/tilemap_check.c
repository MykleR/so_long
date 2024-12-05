/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 19:11:29 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/05 20:29:16 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static uint16_t	exit_check(t_tilemap *map, uint8_t i, uint8_t j, bool *found)
{
	t_tile		tile;

	if (__builtin_expect(!map->tiles || j >= map->size.j
			|| i >= map->size.i, 0))
		return (0);
	tile = tilemap_get(*map, i, j);
	tilemap_set(map, i, j, WALL);
	if (tile == EXIT)
	{
		*found = (map->exit.i == UINT8_MAX);
		map->exit = (t_point){i, j};
	}
	if (tile == WALL)
		return (0);
	return (exit_check(map, i, j + 1, found)
		+ exit_check(map, i - 1, j, found)
		+ exit_check(map, i, j - 1, found)
		+ exit_check(map, i + 1, j, found)
		+ (tile == ITEM));
}

static bool	border_check(t_tilemap map, uint8_t coord, bool axis)
{
	uint8_t	i;
	uint8_t	end;

	i = -1;
	end = map.size.i;
	if (axis)
		end = map.size.j;
	while (++i < end)
	{
		if ((axis && tilemap_get(map, coord, i) != WALL)
			|| (!axis && tilemap_get(map, i, coord) != WALL))
			return (false);
	}
	return (true);
}

static bool	spawn_check(t_tilemap *map, uint16_t *nb_items)
{
	t_tile		tile;
	uint16_t	nb_spawns;
	uint8_t		i;
	uint8_t		j;

	*nb_items = 0;
	nb_spawns = 0;
	i = -1;
	while (++i < map->size.i)
	{
		j = -1;
		while (++j < map->size.j)
		{
			tile = tilemap_get(*map, i, j);
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
	t_tilemap		cpy;
	bool			found_exit;

	if (!border_check(*map, 0, 1) || !border_check(*map, map->size.i - 1, 1)
		|| !border_check(*map, 0, 0) || !border_check(*map, map->size.j - 1, 0))
		return (PARSE_ERROR_NOBOUNDS);
	if (!spawn_check(map, &map->to_collect))
		return (PARSE_ERROR_BADSPAWN);
	if (!map->to_collect)
		return (PARSE_ERROR_NOITEM);
	if (!tilemap_copy(&cpy, *map))
		return (PARSE_ERROR_MEMCRASH);
	if (exit_check(&cpy, map->spawn.i, map->spawn.j,
			&found_exit) != map->to_collect || !found_exit)
	{
		tilemap_destroy(&cpy);
		return (PARSE_ERROR_BADEXIT);
	}
	map->exit = cpy.exit;
	tilemap_destroy(&cpy);
	return (PARSE_OK);
}
