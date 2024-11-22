/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:45:51 by mrouves           #+#    #+#             */
/*   Updated: 2024/11/22 13:29:04 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

#include <time.h>
#include <stdio.h>
#include <math.h>

static float fclampf(float max, float min, float x)
{
	return (fminf(fmaxf(x, min), max));
}

static uint32_t	player_create(t_ecs *ecs, int x, int y, void *texture)
{
	uint32_t id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, TRANSFORM, &((t_vector){x, y}));
	ecs_entity_add(ecs, id, RIGIDBODY, &((t_rigidbody){{0, 0}, {0, 0}, 1}));
	ecs_entity_add(ecs, id, SPRITE, &((t_sprite){texture}));
	return (id);
}

static uint32_t	bullet_create(t_ecs *ecs, void *mlx, uint32_t player, void *texture)
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
		mlx_loop_end(app->mlx);
	else if (t == MLX_KEYDOWN && (e == K_RIGHT || e == K_LEFT))
		bdy->vel.x = ((e == K_RIGHT) - (e == K_LEFT)) * 5;
	else if (t == MLX_KEYDOWN && (e == K_DOWN || e == K_UP))
		bdy->vel.y = ((e == K_DOWN) - (e == K_UP)) * 5;
	else if (t == MLX_KEYUP &&
		((e == K_RIGHT && bdy->vel.x > 0) || (e == K_LEFT && bdy->vel.x < 0)))
		bdy->vel.x = 0;
	else if (t == MLX_KEYUP &&
		((e == K_DOWN && bdy->vel.y > 0) || (e == K_UP && bdy->vel.y < 0)))
		bdy->vel.y = 0;
	else if (t == MLX_MOUSEUP && e == 1)
		env->is_shooting = false;
	else if (t == MLX_MOUSEDOWN && e == 1)
		env->is_shooting = true;
}

void physics_system(t_ecs *ecs, t_ecs_queue **queue, float w, float h)
{
	t_ecs_ulist	query;
	t_vector	*pos;
	t_rigidbody	*bdy;

	query = *ecs_query(ecs, (1ULL << TRANSFORM) | (1ULL << RIGIDBODY));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], TRANSFORM);
		bdy = ecs_entity_get(ecs, query.values[query.len], RIGIDBODY);
		if (!bdy->is_kinematic)
		{
			bdy->vel.x += bdy->accel.x;
			bdy->vel.y += bdy->accel.y;
			bdy->vel.x *= 0.982;
			bdy->vel.y *= 0.982;
			if (!roundf(bdy->vel.x) && !roundf(bdy->vel.y))
				ecs_queue_add(queue, KILL, (t_ecs_queue_payload){0, query.values[query.len], 0});
		}
		pos->x = fclampf(w - 1, 1, pos->x + bdy->vel.x);
		pos->y = fclampf(h - 1, 1, pos->y + bdy->vel.y);
		if (pos->x <= 1 || pos->y <= 1 || pos->x >= w - 1 || pos->y >= h - 1)
			ecs_queue_add(queue, KILL, (t_ecs_queue_payload){0, query.values[query.len], 0});
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
		img = *((t_sprite *)ecs_entity_get(ecs, query.values[query.len], SPRITE));
		pos = *((t_vector *)ecs_entity_get(ecs, query.values[query.len], TRANSFORM));
		mlx_put_image_to_window(mlx, win, img.texture, pos.x, pos.y);
	}
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
	physics_system(env->ecs, &env->queue, app->params.width, app->params.height);
	draw_system(env->ecs, app->mlx, app->win);
	ecs_queue_process(env->ecs, &env->queue);
}

static void	__on_init(t_app *app, t_scene *scene)
{
	t_env		*env;
	uint16_t	comps[63];

	comps[0] = sizeof(t_vector);
	comps[1] = sizeof(t_rigidbody);
	comps[2] = sizeof(t_collider);
	comps[3] = sizeof(t_sprite);
	env = ft_calloc(sizeof(t_env), 1);
	if (!env)
		return ;
	scene->env = env;
	env->ecs = ecs_create(comps, NB_COMPONENTS);
	env->textures[0] = mlx_png_file_to_image(app->mlx, "resources/player.png", NULL, NULL);
	env->textures[1] = mlx_png_file_to_image(app->mlx, "resources/bullet.png", NULL, NULL);
	env->player = player_create(env->ecs, 200, 200, env->textures[0]);
}

static void	__on_clear(t_app *app, t_scene *scene)
{
	t_env	*env;

	env = (t_env *)scene->env;
	if (env)
	{
		mlx_destroy_image(app->mlx, env->textures[0]);
		mlx_destroy_image(app->mlx, env->textures[1]);
		ecs_queue_destroy(&env->queue);
		ecs_destroy(env->ecs);
	}
	free(env);
}

int	main(void)
{
	t_scene	scenes[1];

	scenes[0] = (t_scene){NULL, __on_init, __on_event, __on_update, __on_clear};
	app_autorun((t_win_params){800, 600, 60, "so_long"}, scenes, 1);
	return (0);
}
