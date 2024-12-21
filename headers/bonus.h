/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 23:49:25 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/21 17:36:14 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS_H
# define BONUS_H

# include <application.h>
# include <ecs_extra.h>
# include <collision.h>
# include <parsing.h>
# include <math.h>

# define N_SCENES		3
# define N_COMPS		9
# define N_IMGS_ENV		10
# define N_IMGS_HERO	5
# define N_IMGS_FX		15

# define K_LEFT		4
# define K_RIGHT	7
# define K_UP		26
# define K_DOWN		22
# define K_R		21
# define K_ESCAPE	41

# define S_MENU_FONT_S		50
# define S_PLAYER_SHOOT_F	12
# define S_PLAYER_SHOOT_FOV	45
# define S_PLAYER_FRICTION	0.75
# define S_PLAYER_HP		20
# define S_BULLET_SHOOT_F	8
# define S_BULLET_DURATION	30
# define S_WORLD_GRAVX		0
# define S_WORLD_GRAVY		0.7
# define S_ENEMY_SHOOTRATE	90
# define S_ENEMY_SPEED		1
# define S_ENEMY_BULLET_F	2.5
# define S_ENEMY_HP			10

//
//	============================== ENUMS ==============================
//
// COMPONENTS
typedef enum __attribute__((__packed__)) e_component
{
	COMP_POS,
	COMP_VEL,
	COMP_GRAV,
	COMP_COL,
	COMP_IMG,
	COMP_ANIM,
	COMP_LIFETIME,
	COMP_ENEMY,
	COMP_HP,
}	t_component;

// COLLISIONS
typedef enum __attribute__((__packed__)) e_collider_tag
{
	TAG_BLOCK,
	TAG_PLAYER,
	TAG_EXIT,
	TAG_ENEMY,
	TAG_PBULLET,
	TAG_EBULLET,
	TAG_ITEM,
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

typedef struct s_animation
{
	t_sprite	*frames;
	uint16_t	delay;
	uint16_t	count;
	uint16_t	nb_frames;
	uint16_t	frame;
}	t_animation;

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
	t_sprite	imgs_env[N_IMGS_ENV];
	t_sprite	imgs_hero[N_IMGS_HERO];
	t_sprite	imgs_fx[N_IMGS_FX];
}	t_prog_args;

typedef struct s_game
{
	t_ecs		*ecs;
	t_col_grid	grid;
	t_ecs_queue	queue;
	t_aabb		camera;
	uint32_t	player;
	uint32_t	collected;
	uint32_t	to_collect;
}	t_game;

//	============================== Functions =============================

uint32_t	instantiate_player(t_ecs *ecs, t_sprite sprite,
				float x, float y);
uint32_t	instantiate_enemy(t_ecs *ecs, t_sprite *imgs,
				t_vector pos, uint32_t player_id);
uint32_t	instantiate_pbullet(t_ecs *ecs, t_sprite *sprite,
				t_vector pos, t_vector vel);
uint32_t	instantiate_ebullet(t_ecs *ecs, t_sprite *sprite,
				t_vector pos, t_vector vel);
uint32_t	instantiate_tile(t_ecs *ecs, t_sprite *imgs,
				t_vector pos, t_tile tile);
uint32_t	instantiate_particule(t_ecs *ecs, t_animation anim,
				t_vector pos, int lifetime);

int			unload_app_resources(void *mlx, t_prog_args *args);
int			load_app_resources(void *mlx, t_prog_args *args);

void		game_render(t_app *app, t_game *game);
void		game_physics(t_app *app, t_game *game);
int			__game_init(t_app *app, t_scene *scene);
int			__game_update(t_app *app, t_scene *scene);
int			__game_clear(t_app *app, t_scene *scene);
int			__game_event(t_app *app, t_scene *scene, mlx_event_type t, int e);

int			__menu_init(t_app *app, t_scene *scene);
int			__menu_update(t_app *app, t_scene *scene);
int			__menu_event(t_app *app, t_scene *scene, mlx_event_type t, int e);
int			__death_init(t_app *app, t_scene *scene);
int			__death_update(t_app *app, t_scene *scene);
int			__death_event(t_app *app, t_scene *scene, mlx_event_type t, int e);

void		__player_collide(uint32_t self, uint32_t other, void *data);
void		__bullet_collide(uint32_t self, uint32_t other, void *data);
void		__enemy_collide(uint32_t self, uint32_t other, void *data);
void		__item_collide(uint32_t self, uint32_t other, void *data);
void		__exit_collide(uint32_t self, uint32_t other, void *data);

#endif
