/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:21:11 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/02 21:39:58 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

bool	intersects(t_aabb b, t_aabb a)
{
	return (b.x <= a.x + a.w && b.x + b.w >= a.x
		&& b.y <= a.y + a.h & b.y + b.h >= a.y);
}

static bool	is_ingrid(int16_t row, int16_t col, uint16_t w, uint16_t h)
{
	return (row >= 0 && row < h && col >= 0 && col < w);
}

static void	check_collisions(t_ecs *ecs, t_ecs_ulist l1,
					t_ecs_ulist l2, collide_event callback)
{
	t_collider	*col1;
	t_collider	*col2;
	t_vector	*pos1;
	t_vector	*pos2;
	uint32_t	others;

	while (l1.len--)
	{
		others = l2.len;
		while (others--)
		{
			col1 = ecs_entity_get(ecs, l1.values[l1.len], COLLIDER);
			col2 = ecs_entity_get(ecs, l2.values[others], COLLIDER);
			pos1 = ecs_entity_get(ecs, l1.values[l1.len], TRANSFORM);
			pos2 = ecs_entity_get(ecs, l2.values[others], TRANSFORM);
			if (l2.values[others] != l1.values[l1.len] && intersects(
					(t_aabb){pos1->x, pos1->y, col1->w, col1->h},
				(t_aabb){pos2->x, pos2->y, col2->w, col2->h}))
				callback(ecs, l1.values[l1.len], l2.values[others]);
		}
	}
}

static void	check_neightbours(t_col_grid *grid, t_ecs *ecs,
				collide_event callback, uint32_t i)
{
	static const int	offsets[16] = {-1, -1, -1, 0, 0, 1, 1, 1,
		-1, 0, 1, -1, 1, -1, 0, 1};
	uint32_t			nbour;
	uint32_t			j;

	j = -1;
	while (++j < 8)
	{
		nbour = i + grid->length * offsets[j] + offsets[j + 8];
		if (is_ingrid(nbour / grid->length, nbour % grid->length,
				grid->length, grid->length) && grid->cells[nbour].len)
			check_collisions(ecs, *(grid->cells + i),
				*(grid->cells + nbour), callback);
	}
}

void	grid_process(t_col_grid *grid, t_ecs *ecs, collide_event callback)
{
	uint32_t	i;

	i = -1;
	while (++i < grid->area)
	{
		if ((grid->cells + i)->len)
		{
			check_collisions(ecs, *(grid->cells + i),
				*(grid->cells + i), callback);
			check_neightbours(grid, ecs, callback, i);
		}
	}
	grid_clear(grid);
}
