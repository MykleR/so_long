/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:35:58 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/06 00:07:12 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static bool	parse_line(t_tilemap *map, const char *line, uint8_t i)
{
	uint8_t	j;

	j = 0;
	while (*line && *line != '\n')
	{
		if (!tilemap_set(map, i, j++, *line))
			return (false);
		line++;
	}
	return (j == map->size.j);
}

static t_parse_error	get_bounds(const char *path, uint8_t *w, uint8_t *h)
{
	int			fd;
	char		*line;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (PARSE_ERROR_FILECRASH);
	line = get_next_line(fd);
	*w = ft_strlen(line) - (line && *line);
	*h = 0;
	while (line)
	{
		(*h)++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (PARSE_OK);
}

void	print_parse_error(t_parse_error error)
{
	static const char	*error_messages[9] = {
		"Ok\n",
		"Invalid number of arguments.\n",
		"Couldn't open file.\n",
		"Couldn't create tilemap.\n",
		"Bounds are not closed !\n",
		"Spawn is invalid !\n",
		"Bounds invalid (could be invalid char).\n",
		"Exit is not reachable/found !\n",
		"No collectibles found !\n",
	};

	if (error != PARSE_OK)
		ft_printf(2, "Error\n");
	ft_printf(2, "Invalid map: ");
	ft_printf(2, error_messages[error]);
}

t_parse_error	tilemap_parse(t_tilemap *map, const char *path)
{
	char		*line;
	int			fd;
	uint8_t		i;
	bool		is_rect;

	if (get_bounds(path, &map->size.j, &map->size.i) != PARSE_OK)
		return (PARSE_ERROR_FILECRASH);
	if (!tilemap_create(map, map->size.j, map->size.i))
		return (PARSE_ERROR_MEMCRASH);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (PARSE_ERROR_FILECRASH);
	i = 0;
	is_rect = true;
	line = get_next_line(fd);
	while (line)
	{
		is_rect &= parse_line(map, line, i++);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (is_rect)
		return (PARSE_OK);
	return (PARSE_ERROR_BADSHAPE);
}
