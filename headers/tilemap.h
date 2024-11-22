/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:59:50 by mrouves           #+#    #+#             */
/*   Updated: 2024/11/21 17:10:15 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TILEMAP_H
# define TILEMAP_H

# include "stdint.h"
# include "stdlib.h"

typedef enum e_tile
{
	EMPTY,
	WALL,
	EXIT,
	SPAWN,
} __attribute__((__packed__))	t_tile;

typedef struct s_tilemap
{
	t_tile		*tiles;
	uint16_t	width;
	uint16_t	height;
}	t_tilemap;

t_tilemap	*tilemap_create(uint16_t w, uint16_t h);
void		tilemap_destroy(t_tilemap *tilemap);
void		tilemap_set(t_tilemap *tilemap, uint16_t x, uint16_t y, t_tile tile);
t_tile		tilemap_get(t_tilemap *tilemap, uint16_t x, uint16_t y);
#endif
