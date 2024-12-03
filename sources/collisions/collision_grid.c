/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_grid.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:41:10 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 12:05:43 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manda.h"

bool	grid_create(t_col_grid *grid, t_aabb bounds)
{
	uint32_t	i;
	bool		success;

	i = -1;
	if (__builtin_expect(!grid, 0))
		return (false);
	grid->cell_size = COLGRID_CELL_SIZE;
	grid->length = bounds.w / grid->cell_size;
	grid->area = grid->length * grid->length;
	grid->bounds = (t_aabb){bounds.x, bounds.y,
		grid->length * grid->cell_size,
		grid->length * grid->cell_size};
	grid->cells = ft_calloc(sizeof(t_ecs_ulist), grid->area);
	if (__builtin_expect(!grid->cells, 0))
		return (false);
	success = true;
	while (++i < grid->area && success)
		success = success && list_create(grid->cells + i);
	if (!success)
		grid_destroy(grid);
	return (success);
}

void	grid_destroy(t_col_grid *grid)
{
	uint32_t	i;

	i = -1;
	while (++i < grid->area)
		list_destroy(grid->cells + i);
	free(grid->cells);
	ft_memset(grid, 0, sizeof(t_col_grid));
}

void	grid_clear(t_col_grid *grid)
{
	uint32_t	i;

	i = -1;
	while (++i < grid->area)
		grid->cells[i].len = 0;
}

void	grid_insert(t_col_grid *grid, uint32_t id, int x, int y)
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	index;

	if (__builtin_expect(x <= grid->bounds.x
			|| x >= grid->bounds.x + grid->bounds.w
			|| y <= grid->bounds.y
			|| y >= grid->bounds.y + grid->bounds.h, 0))
		return ;
	j = (x - grid->bounds.x) / grid->cell_size;
	i = (y - grid->bounds.y) / grid->cell_size;
	index = i * grid->length + j;
	if (__builtin_expect(index < grid->area, 1))
		list_add(grid->cells + index, id);
}
