/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:35:58 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 13:29:15 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static void	tilemap_parse_line(t_tilemap *map, const char *line, uint32_t y)
{
	uint32_t	x;

	x = 0;
	while (*line)
	{
		tilemap_set(map, x++, y, (t_tile)(*line));
		line++;
	}
}

static void	tilemap_get_bounds(const char *path, uint32_t *w, uint32_t *h)
{
	int		fd;
	char	*line;

	fd = open(path, O_RDONLY);
	line = get_next_line(fd);
	*w = ft_strlen(line);
	*h = 0;
	while (line)
	{
		(*h)++;
		free(line);
		line = get_next_line(fd);
	}
	if (fd >= 0)
		close(fd);
}

bool	tilemap_parse(t_tilemap *map, const char *path)
{
	char		*line;
	int			fd;
	uint32_t	y;

	tilemap_get_bounds(path, &map->w, &map->h);
	if (!tilemap_create(map, map->w, map->h))
		return (false);
	y = 0;
	fd = open(path, O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		tilemap_parse_line(map, line, y++);
		free(line);
		line = get_next_line(fd);
	}
	if (fd >= 0)
		close(fd);
	return (true);
}
