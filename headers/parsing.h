/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 17:57:36 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/06 14:36:35 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <libft.h>
# include <fcntl.h>

# define TILE_SIZE 32
# define TILEMAP_MAX_SIZE 80

typedef enum e_parse_error
{
	PARSE_OK,
	PARSE_ERROR_NOARGS,
	PARSE_ERROR_FILECRASH,
	PARSE_ERROR_MEMCRASH,
	PARSE_ERROR_NOBOUNDS,
	PARSE_ERROR_BADSPAWN,
	PARSE_ERROR_BADSHAPE,
	PARSE_ERROR_BADEXIT,
	PARSE_ERROR_NOITEM,
}	t_parse_error;

typedef enum e_tile
{
	FLOOR = '0',
	WALL = '1',
	ITEM = 'C',
	EXIT = 'E',
	SPAWN = 'P'
}	t_tile;

typedef struct s_point
{
	uint8_t	i;
	uint8_t	j;
}	t_point;

typedef struct s_tilemap
{
	t_tile		*tiles;
	uint16_t	area;
	uint16_t	to_collect;
	t_point		size;
	t_point		spawn;
	t_point		exit;
}	t_tilemap;

t_parse_error	tilemap_parse(t_tilemap *map, const char *path);
t_parse_error	tilemap_check(t_tilemap *map);
bool			tilemap_copy(t_tilemap *dst, t_tilemap src);
bool			tilemap_create(t_tilemap *map, uint8_t w, uint8_t h);
bool			tilemap_set(t_tilemap *map, uint8_t i, uint8_t j, t_tile t);
t_tile			tilemap_get(t_tilemap map, uint8_t i, uint8_t j);
void			tilemap_destroy(t_tilemap *map);
void			print_parse_error(t_parse_error error);	

#endif
