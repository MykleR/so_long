/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:59:50 by mrouves           #+#    #+#             */
/*   Updated: 2024/11/22 15:35:47 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TILEMAP_H
# define TILEMAP_H

# include "stdint.h"
# include "stdlib.h"

typedef enum __attribute__((packed)) e_tile
{
	EMPTY = 0,
	WALL = 1,
	EXIT = 2,
	SPAWN = 3,
}	t_tile;

typedef struct s_tilemap
{
	t_tile		*tiles;
	uint16_t	width;
	uint16_t	height;
}	t_tilemap;

t_tilemap	*tilemap_create(uint16_t w, uint16_t h);
void		tilemap_destroy(t_tilemap *tilemap);
void		tilemap_set(t_tilemap *tilemap,
				uint16_t x, uint16_t y, t_tile tile);
t_tile		tilemap_get(t_tilemap *tilemap, uint16_t x, uint16_t y);
#endif
