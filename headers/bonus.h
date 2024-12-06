/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 23:49:25 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/06 17:39:44 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS_H
# define BONUS_H

# include <application.h>
# include <ecs.h>
# include <collision.h>
# include <parsing.h>

# define NB_SCENES 2
# define NB_COMPS 7
# define NB_TEXTURES 1

# define K_LEFT 4
# define K_RIGHT 7
# define K_UP 26
# define K_DOWN 22
# define K_R 21
# define K_ESCAPE 41

# define MENU_FONT_SIZE 50

//
//	============================== ENUMS ==============================
//
// COMPONENTS
typedef enum __attribute__((__packed__)) e_component
{
	C_POSITION,
	C_VELOCITY,
	C_COLLIDER,
	C_SPRITE,
	C_ENNEMY,
	C_HEALTH,
	C_DAMAGE,
}	t_component;

// COLLISIONS
typedef enum __attribute__((__packed__)) e_collider_tag
{
	T_BLOCK,
	T_PLAYER,
	T_EXIT,
	T_ENNEMY,
	T_PROJECTILE,
}	t_collider_tag;

//
//	============================== Structs =============================
//
// /!\ Some components are represented as only integers ! (like health)
//
// Vector, used to represent POSITION AND VELOCITY component
// And used in other components
typedef struct s_vector
{
	float	x;
	float	y;
}	t_vector;

// Collider, used to represent COLLIDER component
// uint16_t			w -> Collider aabb width
// uint16_t			h -> Collider aabb height
// t_collider_type	type -> Collider tag
typedef struct s_collider
{
	t_collide_event	on_collide;
	uint16_t		w;
	uint16_t		h;
	t_collider_tag	tag;
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

typedef struct s_game
{
	t_sprite	sprites[NB_TEXTURES];
	t_ecs		*ecs;
	t_col_grid	grid;
	t_aabb		camera;
	uint32_t	player;
	uint32_t	nb_bullets;
	t_vector	gravity;
}	t_game;

//	============================== Functions =============================

int		__game_init(t_app *app, t_scene *scene);
int		__game_update(t_app *app, t_scene *scene);
int		__game_clear(t_app *app, t_scene *scene);
int		__game_event(t_app *app, t_scene *scene, mlx_event_type t, int e);
int		__menu_init(t_app *app, t_scene *scene);
int		__menu_update(t_app *app, t_scene *scene);
int		__menu_clear(t_app *app, t_scene *scene);
int		__menu_event(t_app *app, t_scene *scene, mlx_event_type t, int e);

void	__player_collide(uint32_t self, uint32_t other, void *data);
void	__ennemy_collide(uint32_t self, uint32_t other, void *data);
void	__item_collide(uint32_t self, uint32_t other, void *data);

#endif
