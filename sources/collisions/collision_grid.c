/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_grid.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:41:10 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 23:09:27 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <collision.h>

static bool	grid_fill(t_col_grid *grid)
{
	uint32_t	i;
	bool		success;

	i = -1;
	success = true;
	while (++i < grid->area && success)
		success &= array_list_create(grid->cells + i, sizeof(t_col_info));
	return (success);
}

bool	grid_create(t_col_grid *grid, t_aabb bounds)
{
	if (__builtin_expect(!grid, 0))
		return (false);
	grid->cell_size = COLGRID_CELL_SIZE;
	grid->length = bounds.w / grid->cell_size;
	grid->area = grid->length * grid->length;
	grid->bounds = (t_aabb){bounds.x, bounds.y,
		grid->length * grid->cell_size,
		grid->length * grid->cell_size};
	grid->cells = ft_calloc(sizeof(t_array_list), grid->area);
	if (__builtin_expect(!grid->cells, 0))
		return (false);
	if (__builtin_expect(grid_fill(grid), 1))
		return (true);
	grid_destroy(grid);
	return (false);
}

void	grid_destroy(t_col_grid *grid)
{
	uint32_t	i;

	i = -1;
	while (++i < grid->area)
		array_list_destroy(grid->cells + i);
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

void	grid_insert(t_col_grid *grid, t_aabb bounds, uint32_t id)
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	index;

	if (__builtin_expect(!intersects(bounds, grid->bounds), 0))
		return ;
	j = (bounds.x - grid->bounds.x) / grid->cell_size;
	i = (bounds.y - grid->bounds.y) / grid->cell_size;
	index = i * grid->length + j;
	if (__builtin_expect(index < grid->area, 1))
		array_list_insert(grid->cells + index, &((t_col_info){bounds, id}));
}
