/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_grid.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:41:10 by mrouves           #+#    #+#             */
/*   Updated: 2024/11/29 14:27:53 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static bool	grid_fill(t_col_grid *grid)
{
	uint32_t	i;

	grid->cells = ft_calloc(sizeof(t_ecs_ulist *), grid->area);
	if (__builtin_expect(!grid->cells, 0))
		return (false);
	i = -1;
	while (++i < grid->area)
	{
		*(grid->cells + i) = list_create(COLGRID_CELL_INIT);
		if (!(*(grid->cells + i)))
		{
			free(grid->cells);
			return (false);
		}
	}
	return (true);
}

void	grid_create(t_col_grid *grid, t_aabb bounds)
{
	if (__builtin_expect(!grid, 0))
		return ;
	grid->cell_size = COLGRID_CELL_SIZE;
	grid->length = bounds.w / grid->cell_size;
	grid->area = grid->length * grid->length;
	grid->bounds = (t_aabb){bounds.x, bounds.y,
		grid->length * grid->cell_size,
		grid->length * grid->cell_size};
	grid_fill(grid);
}

void	grid_destroy(t_col_grid *grid)
{
	uint32_t	i;

	i = -1;
	while (++i < grid->area)
		list_destroy(*(grid->cells + i));
	free(grid->cells);
}

void	grid_clear(t_col_grid *grid)
{
	uint32_t	i;

	i = -1;
	while (++i < grid->area)
	{
		ft_memset(grid->cells[i]->values, 0,
			sizeof(uint32_t) * grid->cells[i]->len);
		grid->cells[i]->len = 0;
	}
}

void	grid_insert(t_col_grid *grid, uint32_t id, t_vector pos)
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	index;

	if (__builtin_expect(pos.x <= grid->bounds.x
			|| pos.x >= grid->bounds.x + grid->bounds.w
			|| pos.y <= grid->bounds.y
			|| pos.y >= grid->bounds.y + grid->bounds.h, 0))
		return ;
	j = floor((pos.x - grid->bounds.x) / grid->cell_size);
	i = floor((pos.y - grid->bounds.y) / grid->cell_size);
	index = i * grid->length + j;
	if (__builtin_expect(index < grid->area, 1))
		list_add(grid->cells[index], id);
}
