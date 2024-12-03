/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:44:14 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 12:26:48 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLISION_H
# define COLLISION_H

# include "ecs.h"

# define COLGRID_CELL_INIT 16
# define COLGRID_CELL_SIZE 50

// COLLISIONS
typedef enum __attribute__((__packed__)) e_collider_type
{
	STATIONARY,
	PLAYER,
	PROJECTILE,
	ENNEMY,
}	t_collider_type;

typedef struct s_aabb
{
	int	x;
	int	y;
	int	w;
	int	h;
}	t_aabb;

typedef struct s_col_grid
{
	t_ecs_ulist	*cells;
	uint32_t	area;
	uint16_t	length;
	uint16_t	cell_size;
	t_aabb		bounds;
}	t_col_grid;

typedef void	(*t_collide_event) (t_ecs *, uint32_t, uint32_t);

bool	grid_create(t_col_grid *grid, t_aabb bounds);
void	grid_destroy(t_col_grid *grid);
void	grid_clear(t_col_grid *grid);
void	grid_insert(t_col_grid *grid, uint32_t id, int x, int y);
void	grid_process(t_col_grid *grid, t_ecs *ecs, t_collide_event callback);
bool	intersects(t_aabb a, t_aabb b);

#endif
