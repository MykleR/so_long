/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 23:49:25 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/18 00:11:36 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS_H
# define BONUS_H

# include <application.h>
# include <ecs_extra.h>
# include <collision.h>
# include <parsing.h>
# include <math.h>

# define NB_SCENES			2
# define NB_COMPS			7
# define NB_IMGS_ENV		1
# define NB_IMGS_BG			1
# define NB_IMGS_HERO		2

# define K_LEFT		4
# define K_RIGHT	7
# define K_UP		26
# define K_DOWN		22
# define K_R		21
# define K_ESCAPE	41

# define S_MENU_FONT_S		50
# define S_PLAYER_SHOOT_F	12
# define S_PLAYER_SHOOT_NB	6
# define S_PLAYER_SHOOT_FOV	60
# define S_PLAYER_FRICTION	0.8
# define S_BULLET_SHOOT_F	20

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
	C_GRAVITY
}	t_component;

// COLLISIONS
typedef enum __attribute__((__packed__)) e_collider_tag
{
	T_BLOCK,
	T_PLAYER,
	T_EXIT,
	T_ENNEMY,
	T_PROJECTILE,
	T_ITEM,
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

typedef struct s_box_resolve
{
	t_collider	*c1;
	t_collider	*c2;
	t_vector	*p1;
	t_vector	*p2;
}	t_box_resolve;

typedef struct s_prog_args
{
	char		**argv;
	int			argc;
	t_tilemap	tilemap;
	t_sprite	imgs_env[NB_IMGS_ENV];
	t_sprite	imgs_hero[NB_IMGS_HERO];
	t_sprite	imgs_bg[NB_IMGS_BG];
}	t_prog_args;

typedef struct s_game
{
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
void	__bullet_collide(uint32_t self, uint32_t other, void *data);
void	__ennemy_collide(uint32_t self, uint32_t other, void *data);
void	__item_collide(uint32_t self, uint32_t other, void *data);

#endif
