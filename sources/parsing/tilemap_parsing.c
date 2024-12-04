/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:35:58 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 22:06:31 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static void	tilemap_parse_line(t_tilemap *map, const char *line, uint16_t i)
{
	uint16_t	j;

	j = 0;
	while (*line)
	{
		tilemap_set(map, i, j++, *line);
		line++;
	}
}

static void	tilemap_get_bounds(const char *path, uint16_t *w, uint16_t *h)
{
	int		fd;
	char	*line;

	fd = open(path, O_RDONLY);
	line = get_next_line(fd);
	*w = ft_strlen(line) - (line && *line);
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
	uint16_t	i;

	tilemap_get_bounds(path, &map->w, &map->h);
	if (!tilemap_create(map, map->w, map->h))
		return (false);
	i = 0;
	fd = open(path, O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		tilemap_parse_line(map, line, i++);
		free(line);
		line = get_next_line(fd);
	}
	if (fd >= 0)
		close(fd);
	return (true);
}
