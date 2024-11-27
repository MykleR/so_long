/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualization.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:58:46 by mykle             #+#    #+#             */
/*   Updated: 2024/11/27 20:19:56 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"
#include "ecs.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

#define POSITION 0
#define VELOCITY 1
#define COLLIDER 2

typedef enum e_collision_tag
{
	STATIC,
	MOVING,
}	t_collision_tag;

typedef struct s_collider
{
	uint16_t		w;
	uint16_t		h;
	t_collision_tag	tag;
	bool			is_colliding;
}	t_collider;

typedef struct s_vector
{
	float	x;
	float	y;
}	t_vector;

typedef struct	s_env
{
	t_ecs	*ecs;
}	t_env;

float	fclampf(float x, float max, float min)
{
	return (fmaxf(fminf(max, x), min));
}

int	wich_quadrent(t_vector pos, t_vector center)
{
	if (pos.y < center.y)
		return (pos.x >= center.x);
	return ((pos.x >= center.x) + 2);
}

bool	is_in_bound(t_vector pos, t_vector center, int w, int h)
{
	int hw;
	int hh;

	hw = w >> 1;
	hh = h >> 1;
	return (pos.x >= center.x - hw && pos.x <= center.x + hw
		&& pos.y >= center.y - hh && pos.y <= center.y + hh);
}

static uint32_t box_create(t_ecs *ecs, float x, float y, float vx, float vy)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, POSITION, &(t_vector){x, y});
	ecs_entity_add(ecs, id, VELOCITY, &(t_vector){vx , vy});
	ecs_entity_add(ecs, id, COLLIDER, &(t_collider){10, 10, MOVING, 0});
	return (id);
}

void	box_draw(t_app *app, t_vector pos, int w, int h, uint32_t color)
{
	int16_t	i;
	int		ww;
	int		wh;

	ww = app->params.width;
	wh = app->params.height;
	mlx_pixel_put(app->mlx, app->win, pos.x, pos.y, color);
	i = -(w >> 1) - 1;
	while (++i < (w >> 1))
	{
		if (is_in_bound((t_vector){pos.x + i, pos.y - (h >> 1)}, (t_vector){ww >> 1, wh >> 1}, ww - 1, wh - 1))
			mlx_pixel_put(app->mlx, app->win, pos.x + i, pos.y - (h >> 1), color);
		if (is_in_bound((t_vector){pos.x + i, pos.y + (h >> 1)}, (t_vector){ww >> 1, wh >> 1}, ww - 1, wh - 1))
			mlx_pixel_put(app->mlx, app->win, pos.x + i, pos.y + (h >> 1), color);
	}
	i = -(h >> 1) - 1;
	while (++i < (h >> 1))
	{
		if (is_in_bound((t_vector){pos.x - (w >> 1), pos.y + i}, (t_vector){ww >> 1, wh >> 1}, ww - 1, wh - 1))
			mlx_pixel_put(app->mlx, app->win, pos.x - (w >> 1), pos.y + i, color);
		if (is_in_bound((t_vector){pos.x + (w >> 1), pos.y + i}, (t_vector){ww >> 1, wh >> 1}, ww - 1, wh - 1))
			mlx_pixel_put(app->mlx, app->win, pos.x + (w >> 1), pos.y + i, color);
	}
}

void	box_system(t_ecs *ecs, t_app *app)
{
	t_ecs_ulist	query;
	t_vector	*pos;
	t_vector	*vel;
	t_collider	*col;

	query = *(ecs_query(ecs, POSITION | VELOCITY | COLLIDER));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], POSITION);
		vel = ecs_entity_get(ecs, query.values[query.len], VELOCITY);
		col = ecs_entity_get(ecs, query.values[query.len], COLLIDER);
		pos->x = fclampf(pos->x + vel->x, app->params.width - (col->w >> 1) - 1, (col->w >> 1) + 1);
		pos->y = fclampf(pos->y + vel->y, app->params.height - (col->h >> 1) - 1, (col->h >> 1) + 1);
		if (pos->x - (col->w >> 1) < (col->w >> 1) + 1 || pos->x + (col->w >> 1) > app->params.width - (col->w >> 1) - 1)
			vel->x = -vel->x;
		if (pos->y - (col->h >> 1) < (col->h >> 1) + 1 || pos->y + (col->h >> 1) > app->params.height - (col->h >> 1) - 1)
			vel->y = -vel->y;
		box_draw(app, *pos, col->w, col->h, col->is_colliding ? 0xFFFF0000 : 0xFFFFFFFF);
	}
}

static void	__init(t_app *app, t_scene *scene)
{
	uint16_t	i;
	t_env		*env;

	(void) app;
	env = malloc(sizeof(t_env));
	if (!env)
		return ;
	scene->env = env;
	env->ecs = ecs_create(3, sizeof(t_vector),
					sizeof(t_vector), sizeof(t_collider));
	srand(time(NULL));
	i = -1;
	while (++i < ECS_ENTITY_CAP - 1)
		box_create(env->ecs,
			 ((float)rand()/(float)(RAND_MAX)) * app->params.width,
			 ((float)rand()/(float)(RAND_MAX)) * app->params.height,
			 ((float)rand()/(float)(RAND_MAX)) * 10 - 5,
			 ((float)rand()/(float)(RAND_MAX)) * 10 - 5);
}

static void	__update(t_app *app, t_scene *scene)
{
	t_env	*env;

	env = (t_env *)scene->env;
	(void) env;
	(void) app;
	mlx_clear_window(app->mlx, app->win);
	box_system(env->ecs, app);
}

static void	__event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	(void) scene;
	if (t == MLX_KEYDOWN && e == 41)
		mlx_loop_end(app->mlx);
}

static void	__clear(t_app *app, t_scene *scene)
{
	(void) app;
	if (scene->env)
		ecs_destroy(((t_env *)scene->env)->ecs);
	free(scene->env);
}

int	main(void)
{
	t_scene	scenes[1];

	scenes[0] = (t_scene){NULL, "main", __init, __event, __update, __clear};
	app_autorun((t_win_params){1280, 720, 60, "collisions"}, scenes, 1);
}
