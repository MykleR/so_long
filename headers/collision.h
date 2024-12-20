/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:44:14 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/19 23:26:40 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLISION_H
# define COLLISION_H

# include <stdint.h>
# include <stdbool.h>

# include <array_list.h>

# define COLGRID_CELL_INIT 16
# define COLGRID_CELL_SIZE 32

typedef void	(*t_collide_event) (uint32_t, uint32_t, void *);

typedef struct s_aabb
{
	int	x;
	int	y;
	int	w;
	int	h;
}	t_aabb;

typedef struct s_col_info
{
	t_collide_event	on_collide;
	t_aabb			bounds;
	uint32_t		id;
}	t_col_info;

typedef struct s_col_grid
{
	t_array_list	*cells;
	uint32_t		area;
	uint16_t		length;
	uint16_t		cell_size;
	t_aabb			bounds;
}	t_col_grid;

bool	grid_create(t_col_grid *grid, t_aabb bounds);
void	grid_destroy(t_col_grid *grid);
void	grid_clear(t_col_grid *grid);
void	grid_insert(t_col_grid *grid, t_collide_event callback,
			t_aabb bounds, uint32_t id);
void	grid_process(t_col_grid *grid, void *data);
bool	intersects(t_aabb a, t_aabb b);

#endif
