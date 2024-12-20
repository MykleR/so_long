/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 23:49:25 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/20 18:14:09 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS_H
# define BONUS_H

# include <application.h>
# include <ecs_extra.h>
# include <collision.h>
# include <parsing.h>
# include <math.h>

# define N_SCENES			2
# define N_COMPS			9
# define N_IMGS_ENV			1
# define N_IMGS_BG			1
# define N_IMGS_HERO		2
# define N_IMGS_OTHER		8

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
# define S_BULLET_SHOOT_F	5
# define S_WORLD_GRAVX		0
# define S_WORLD_GRAVY		0.7

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
	COMP_TARGET,
	COMP_HP,
	COMP_DPS,
}	t_component;

// COLLISIONS
typedef enum __attribute__((__packed__)) e_collider_tag
{
	TAG_BLOCK,
	TAG_PLAYER,
	TAG_EXIT,
	TAG_ENNEMY,
	TAG_BULLET,
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
	uint8_t		nb_frames;
	uint8_t		frame;
	uint8_t		play;
	uint8_t		replay;
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
	t_sprite	imgs_bg[N_IMGS_BG];
	t_sprite	imgs_other[N_IMGS_OTHER];
}	t_prog_args;

typedef struct s_game
{
	t_ecs		*ecs;
	t_col_grid	grid;
	t_ecs_queue	queue;
	t_aabb		camera;
	uint32_t	player;
}	t_game;

//	============================== Functions =============================

uint32_t	instantiate_player(t_ecs *ecs, t_sprite sprite,
				float x, float y);
uint32_t	instantiate_bullet(t_ecs *ecs, t_sprite *sprite,
				t_vector pos, t_vector vel);
uint32_t	instantiate_tile(t_ecs *ecs, t_sprite *imgs,
				t_vector pos, t_tile tile);
uint32_t	instantiate_particule(t_ecs *ecs, t_sprite *imgs, t_vector pos);

int			unload_app_resources(void *mlx, t_prog_args *args);
int			load_app_resources(void *mlx, t_prog_args *args);

void		game_render(t_app *app, t_game *game);
void		game_physics(t_scene *scene, t_game *game);
int			__game_init(t_app *app, t_scene *scene);
int			__game_update(t_app *app, t_scene *scene);
int			__game_clear(t_app *app, t_scene *scene);
int			__game_event(t_app *app, t_scene *scene, mlx_event_type t, int e);

int			__menu_init(t_app *app, t_scene *scene);
int			__menu_update(t_app *app, t_scene *scene);
int			__menu_clear(t_app *app, t_scene *scene);
int			__menu_event(t_app *app, t_scene *scene, mlx_event_type t, int e);

void		__player_collide(uint32_t self, uint32_t other, void *data);
void		__bullet_collide(uint32_t self, uint32_t other, void *data);
void		__ennemy_collide(uint32_t self, uint32_t other, void *data);
void		__item_collide(uint32_t self, uint32_t other, void *data);

#endif
