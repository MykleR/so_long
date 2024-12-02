/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 21:00:34 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 00:09:14 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <math.h>
# include "application.h"
# include "ecs_extra.h"
# include "libft.h"

# define NB_COMPONENTS 4
# define NB_TEXTURES 3

# define COLGRID_CELL_INIT 16
# define COLGRID_CELL_SIZE 50

# define K_LEFT 4
# define K_RIGHT 7
# define K_UP 26
# define K_DOWN 22
# define K_ESCAPE 41

//
//	============================== ENUMS ==============================
//
// COMPONENTS
typedef enum __attribute__((__packed__)) e_component
{
	TRANSFORM,
	RIGIDBODY,
	COLLIDER,
	SPRITE,
}	t_component;

// COLLISIONS
typedef enum __attribute__((__packed__)) e_collider_type
{
	STATIONARY,
	PLAYER,
	PROJECTILE,
	ENNEMY,
}	t_collider_type;

//
//	============================== Structs =============================
//
// Vector Struct, used to represent POSITION component
// And used in other components
typedef struct s_vector
{
	float	x;
	float	y;
}	t_vector;

// Rigidbody, used to represent REGIDBODY component
// t_vector	vel -> Body velocity
// t_vector	accel -> Body acceleration
// bool		kinematic -> Is body kinematic, don't use accel
typedef struct s_rigidbody
{
	t_vector	vel;
	t_vector	accel;
	bool		kinematic;
}	t_rigidbody;

// Collider, used to represent COLLIDER component
// uint16_t			w -> Collider aabb width
// uint16_t			h -> Collider aabb height
// t_collider_type	type -> Collider tag
typedef struct s_collider
{
	uint16_t		w;
	uint16_t		h;
	t_collider_type	type;
}	t_collider;

// Sprite, used to represet SPRITE component
// void	*texture -> the texture displayed on screen
typedef struct s_sprite
{
	void	*texture;
}	t_sprite;

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

typedef struct s_game_env
{
	void		*textures[NB_TEXTURES];
	t_ecs		*ecs;
	t_ecs_queue	queue;
	t_col_grid	grid;
	uint32_t	player;
	bool		is_shooting;
}			t_env;

//	============================== Functions =============================

typedef void	(*t_collide_event) (t_ecs *, uint32_t, uint32_t);

bool	grid_create(t_col_grid *grid, t_aabb bounds);
void	grid_destroy(t_col_grid *grid);
void	grid_clear(t_col_grid *grid);
void	grid_insert(t_col_grid *grid, uint32_t id, t_vector pos);
void	grid_process(t_col_grid *grid, t_ecs *ecs, t_collide_event callback);
bool	intersects(t_aabb a, t_aabb b);

#endif
