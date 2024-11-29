/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:45:51 by mrouves           #+#    #+#             */
/*   Updated: 2024/11/29 15:36:30 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

#include <time.h>
#include <stdio.h>
#include <math.h>

float	fclampf(float max, float min, float x)
{
	return (fminf(fmaxf(x, min), max));
}

static uint32_t	player_create(t_ecs *ecs, int x, int y, void *texture)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, TRANSFORM, &((t_vector){x, y}));
	ecs_entity_add(ecs, id, RIGIDBODY, &((t_rigidbody){{0, 0}, {0, 0}, 1}));
	ecs_entity_add(ecs, id, SPRITE, &((t_sprite){texture}));
	return (id);
}

static uint32_t	wall_create(t_ecs *ecs, t_aabb bounds, void *texture)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, TRANSFORM, &((t_vector){bounds.x, bounds.y}));
	ecs_entity_add(ecs, id, SPRITE, &((t_sprite){texture}));
	ecs_entity_add(ecs, id, COLLIDER, &((t_collider){bounds.w, bounds.h, STATIONARY}));
	return (id);
}

static uint32_t	bullet_create(t_ecs *ecs, void *mlx,
					uint32_t player, void *texture)
{
	int			my;
	int			mx;
	float		a;
	uint32_t	id;
	t_vector	pos;

	mlx_mouse_get_pos(mlx, &mx, &my);
	pos = *((t_vector *)ecs_entity_get(ecs, player, TRANSFORM));
	a = atan2f(my - pos.y, mx - pos.x);
	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, TRANSFORM, &((t_vector){pos.x, pos.y}));
	ecs_entity_add(ecs, id, RIGIDBODY,
		&((t_rigidbody){{cos(a) * 10, sin(a) * 10}, {0, 0}, 0}));
	ecs_entity_add(ecs, id, SPRITE, &((t_sprite){texture}));
	ecs_entity_add(ecs, id, COLLIDER, &((t_collider){10, 10, PROJECTILE}));
	return (id);
}

static void	__on_event(t_app *app, t_scene *scene,
					mlx_event_type t, int e)
{
	t_env		*env;
	t_rigidbody	*bdy;

	env = (t_env *)scene->env;
	bdy = ecs_entity_get(env->ecs, env->player, RIGIDBODY);
	if (t == MLX_KEYDOWN && e == K_ESCAPE)
		app_load(app, 0);
	else if (t == MLX_KEYDOWN && (e == K_RIGHT || e == K_LEFT))
		bdy->vel.x = ((e == K_RIGHT) - (e == K_LEFT)) * 5;
	else if (t == MLX_KEYDOWN && (e == K_DOWN || e == K_UP))
		bdy->vel.y = ((e == K_DOWN) - (e == K_UP)) * 5;
	else if (t == MLX_KEYUP && ((e == K_RIGHT && bdy->vel.x > 0)
			|| (e == K_LEFT && bdy->vel.x < 0)))
		bdy->vel.x = 0;
	else if (t == MLX_KEYUP && ((e == K_DOWN && bdy->vel.y > 0)
			|| (e == K_UP && bdy->vel.y < 0)))
		bdy->vel.y = 0;
	else if (t == MLX_MOUSEUP && e == 1)
		env->is_shooting = false;
	else if (t == MLX_MOUSEDOWN && e == 1)
		env->is_shooting = true;
}

static void	move_system(t_ecs *ecs)
{
	t_ecs_ulist	query;
	t_vector	*pos;
	t_rigidbody	*bdy;

	query = *ecs_query(ecs, (1ULL << TRANSFORM) | (1ULL << RIGIDBODY));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], TRANSFORM);
		bdy = ecs_entity_get(ecs, query.values[query.len], RIGIDBODY);
		bdy->vel.x += bdy->accel.x;
		bdy->vel.y += bdy->accel.y;
		pos->x = pos->x + bdy->vel.x;
		pos->y = pos->y + bdy->vel.y;
	}
}

static void collide_system(t_ecs *ecs, t_col_grid *grid, t_ecs_queue *queue)
{
	t_ecs_ulist	query;
	t_vector	*pos;
	t_collider	*col;

	query = *ecs_query(ecs, (1ULL << TRANSFORM) | (1ULL << COLLIDER));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], TRANSFORM);
		col = ecs_entity_get(ecs, query.values[query.len], COLLIDER);
		if (col->type == PROJECTILE 
				&& !intersects(grid->bounds, (t_aabb){pos->x, pos->y, col->w, col->h}))
			ecs_queue_add(queue, (t_ecs_queue_entry){0, query.values[query.len], 0, KILL});
		else
			grid_insert(grid, query.values[query.len], *pos);
	}
}

void	draw_system(t_ecs *ecs, void *mlx, void *win)
{
	t_ecs_ulist	query;
	t_sprite	img;
	t_vector	pos;

	query = *ecs_query(ecs, (1ULL << SPRITE) | (1ULL << TRANSFORM));
	while (query.len--)
	{
		img = *((t_sprite *)ecs_entity_get(ecs,
					query.values[query.len], SPRITE));
		pos = *((t_vector *)ecs_entity_get(ecs,
					query.values[query.len], TRANSFORM));
		mlx_put_image_to_window(mlx, win, img.texture, pos.x, pos.y);
	}
}

void __on_collide(t_ecs *ecs, uint32_t a, uint32_t b)
{
	t_collider	*col1 = ecs_entity_get(ecs, a, COLLIDER);
	t_collider	*col2 = ecs_entity_get(ecs, b, COLLIDER);
	if (col1->type == col2->type)
		return ;
	if (col1->type == STATIONARY && col2->type == PROJECTILE)
		ecs_entity_kill(ecs, b);
}

static void	__on_update(t_app *app, t_scene *scene)
{
	t_env		*env;

	env = (t_env *)scene->env;
	if (!env)
		return ;
	mlx_clear_window(app->mlx, app->win);
	if (env->is_shooting)
		bullet_create(env->ecs, app->mlx, env->player, env->textures[1]);
	move_system(env->ecs);
	collide_system(env->ecs, &env->grid, &env->queue);
	draw_system(env->ecs, app->mlx, app->win);
	grid_process(&env->grid, env->ecs, __on_collide);
	ecs_queue_process(env->ecs, &env->queue);
}

static void	__on_init(t_app *app, t_scene *scene)
{
	uint32_t	i;
	t_env		*env;

	env = ft_calloc(sizeof(t_env), 1);
	if (!env)
		return ;
	env->ecs = ecs_create(NB_COMPONENTS,
			sizeof(t_vector), sizeof(t_rigidbody),
			sizeof(t_collider), sizeof(t_sprite));
	env->textures[0] = mlx_png_file_to_image(app->mlx,
			"resources/player.png", NULL, NULL);
	env->textures[1] = mlx_png_file_to_image(app->mlx,
			"resources/bullet.png", NULL, NULL);
	env->textures[2] = mlx_png_file_to_image(app->mlx,
			"resources/wall.png", NULL, NULL);
	env->player = player_create(env->ecs, 200, 200, env->textures[0]);
	srand(time(NULL));
	i = -1;
	while (++i < 10)
		wall_create(env->ecs, (t_aabb){
			(rand()/(float)RAND_MAX) * app->params.width,
			(rand()/(float)RAND_MAX) * app->params.height, 32, 32}, env->textures[2]);
	ecs_queue_create(&env->queue, ECS_ENTITY_CAP);
	grid_create(&env->grid, (t_aabb){0, 0, app->params.width, app->params.height});
	scene->env = env;
}

static void	__on_clear(t_app *app, t_scene *scene)
{
	t_env	*env;

	env = (t_env *)scene->env;
	if (!env)
		return ;
	mlx_destroy_image(app->mlx, env->textures[0]);
	mlx_destroy_image(app->mlx, env->textures[1]);
	mlx_destroy_image(app->mlx, env->textures[2]);
	grid_destroy(&env->grid);
	ecs_queue_destroy(&env->queue);
	ecs_destroy(env->ecs);
	free(env);
}

static void	__scene2_update(t_app *app, t_scene *scene)
{
	(void) scene;
	mlx_string_put(app->mlx, app->win, (app->params.width >> 1) - 100, app->params.height >> 1, 0xFFFFFFFF, "MAIN MENU");
}

static void	__scene2_init(t_app *app, t_scene *scene)
{
	(void) scene;
	mlx_set_font_scale(app->mlx, app->win, "resources/pixelated.ttf", 50);
}

static void	__scene2_event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	(void) scene;
	if (t == MLX_MOUSEDOWN && e == 1)
		app_load(app, 1);
	else if (t == MLX_KEYDOWN && e == K_ESCAPE)
		mlx_loop_end(app->mlx);
}

int	main(void)
{
	t_scene	scenes[2];

	scenes[1] = (t_scene){NULL, "game", __on_init, __on_event, __on_update, __on_clear};
	scenes[0] = (t_scene){NULL, "menu", __scene2_init, __scene2_event, __scene2_update, NULL};
	app_autorun((t_win_params){800, 600, 60, "so_long"}, scenes, 2);
	return (0);
}
