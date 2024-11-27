/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 21:00:34 by mrouves           #+#    #+#             */
/*   Updated: 2024/11/27 15:52:43 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "application.h"
# include "ecs_extra.h"

# define NB_COMPONENTS 4

# define K_LEFT 4
# define K_RIGHT 7
# define K_UP 26
# define K_DOWN 22
# define K_ESCAPE 41

typedef enum e_component
{
	TRANSFORM,
	RIGIDBODY,
	COLLIDER,
	SPRITE,
}	t_component;

typedef enum __attribute__((__packed__)) e_collider_type
{
	STATIONARY,
	PLAYER,
	PROJECTILE,
	ENNEMY,
}	t_collider_type;

typedef struct s_vector
{
	float	x;
	float	y;
}	t_vector;

typedef struct s_rigidbody
{
	t_vector	vel;
	t_vector	accel;
	bool		kinematic;
}	t_rigidbody;

typedef struct s_collider
{
	t_vector		box;
	t_collider_type	type;
}	t_collider;

typedef struct s_sprite
{
	void	*texture;
}	t_sprite;

typedef struct s_tilemap
{
	uint8_t		*tiles;
	uint16_t	width;
	uint16_t	height;
}	t_tilemap;

typedef struct s_env
{
	void		*textures[2];
	t_ecs_queue	queue;
	uint32_t	player;
	bool		is_shooting;
	t_ecs		*ecs;
}			t_env;

#endif
