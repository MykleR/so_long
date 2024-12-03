/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manda.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:37:07 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 20:59:15 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANDA_H
# define MANDA_H

# include "ecs.h"
# include "libft.h"

# include "application.h"
# include "collision.h"
# include "parsing.h"

# define NB_SCENES 1
# define NB_COMPONENTS 3
# define NB_TEXTURES 2
# define SPRITE_SIZE 32

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
	COLLIDER,
	SPRITE,
}	t_component;

//
//	============================== Structs =============================
//
// Vector, used to represent TRANSFORM component
// And used in other components
typedef struct s_vector
{
	int	x;
	int	y;
}	t_vector;

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
	int		w;
	int		h;
}	t_sprite;

typedef struct s_prog_args
{
	char		**argv;
	int			argc;
	t_tilemap	tilemap;
}	t_prog_args;

typedef struct s_env
{
	t_sprite	textures[NB_TEXTURES];
	t_ecs		*ecs;
	t_col_grid	grid;
	t_aabb		camera;
	t_tilemap	tilemap;
	uint32_t	player;
	t_vector	last_pos;
}			t_env;

//	============================== Functions =============================

int		__on_init(t_app *app, t_scene *scene);
int		__on_event(t_app *app, t_scene *scene, mlx_event_type t, int e);
int		__on_update(t_app *app, t_scene *scene);
int		__on_clear(t_app *app, t_scene *scene);
#endif
