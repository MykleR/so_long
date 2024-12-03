/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:21:11 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 23:09:23 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <collision.h>

bool	intersects(t_aabb b, t_aabb a)
{
	return (b.x < a.x + a.w && b.x + b.w > a.x
		&& b.y < a.y + a.h & b.y + b.h > a.y);
}

static bool	is_ingrid(int16_t row, int16_t col, uint16_t w, uint16_t h)
{
	return (row >= 0 && row < h && col >= 0 && col < w);
}

static void	check_collisions(t_array_list *l1, t_array_list *l2,
				t_collide_event callback, void *data)
{
	t_col_info	*col1;
	t_col_info	*col2;
	uint32_t	i;
	uint32_t	j;

	i = -1;
	while (++i < l1->len)
	{
		col1 = array_list_get(l1, i);
		j = -1;
		while (++j < l2->len)
		{
			col2 = array_list_get(l2, j);
			if (col1->id != col2->id && intersects(col1->bounds, col2->bounds))
				callback(col1->id, col2->id, data);
		}
	}
}

static void	check_neightbours(t_col_grid *grid, uint32_t i,
				t_collide_event callback, void *data)
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
			check_collisions(grid->cells + i, grid->cells + nbour,
				callback, data);
	}
}

void	grid_process(t_col_grid *grid, t_collide_event callback, void *data)
{
	uint32_t	i;

	i = -1;
	while (++i < grid->area)
	{
		if ((grid->cells + i)->len)
		{
			check_collisions(grid->cells + i, grid->cells + i, callback, data);
			check_neightbours(grid, i, callback, data);
		}
	}
	grid_clear(grid);
}
