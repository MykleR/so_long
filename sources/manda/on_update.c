/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:24:01 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 13:27:19 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

static void	collide_system(t_ecs *ecs, t_col_grid *grid)
{
	t_ecs_ulist	query;
	t_vector	*pos;
	t_collider	*col;

	query = *ecs_query(ecs, (1ULL << TRANSFORM) | (1ULL << COLLIDER));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], TRANSFORM);
		col = ecs_entity_get(ecs, query.values[query.len], COLLIDER);
		grid_insert(grid, (t_aabb){pos->x, pos->y, col->w, col->h},
			query.values[query.len]);
	}
}

static void	draw_system(t_ecs *ecs, void *mlx, void *win, t_aabb cam)
{
	t_ecs_ulist	query;
	t_sprite	*img;
	t_vector	*pos;

	query = *ecs_query(ecs, (1ULL << SPRITE) | (1ULL << TRANSFORM));
	while (query.len--)
	{
		img = ecs_entity_get(ecs, query.values[query.len], SPRITE);
		pos = ecs_entity_get(ecs, query.values[query.len], TRANSFORM);
		if (intersects((t_aabb){pos->x, pos->y, img->w, img->h}, cam))
			mlx_put_image_to_window(mlx, win, img->texture,
				pos->x - cam.x, pos->y - cam.y);
	}
}

static void	__on_collide(uint32_t a, uint32_t b, void *ptr)
{
	t_env		*env;
	t_collider	*col1;
	t_collider	*col2;
	t_vector	*pos1;

	env = (t_env *)((t_scene *)ptr)->env;
	col1 = ecs_entity_get(env->ecs, a, COLLIDER);
	col2 = ecs_entity_get(env->ecs, b, COLLIDER);
	if (col1->type == col2->type)
		return ;
	pos1 = ecs_entity_get(env->ecs, a, TRANSFORM);
	if (col1->type == PLAYER && col2->type == STATIC)
		*pos1 = env->last_pos;
}

int	__on_update(t_app *app, t_scene *scene)
{
	t_env		*env;
	t_vector	*pos;

	env = (t_env *)scene->env;
	mlx_clear_window(app->mlx, app->win);
	collide_system(env->ecs, &env->grid);
	grid_process(&env->grid, __on_collide, scene);
	pos = ecs_entity_get(env->ecs, env->player, TRANSFORM);
	env->camera.x = pos->x - env->camera.w / 2;
	env->camera.y = pos->y - env->camera.h / 2;
	draw_system(env->ecs, app->mlx, app->win, env->camera);
	return (0);
}
