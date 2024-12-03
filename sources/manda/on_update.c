/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:24:01 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 16:24:14 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manda.h"

static void collide_system(t_ecs *ecs, t_col_grid *grid)
{
	t_ecs_ulist	query;
	t_vector	*pos;

	query = *ecs_query(ecs, (1ULL << TRANSFORM) | (1ULL << COLLIDER));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], TRANSFORM);
		grid_insert(grid, query.values[query.len], pos->x, pos->y);
	}
}

static void	draw_system(t_ecs *ecs, void *mlx, void *win)
{
	t_ecs_ulist	query;
	t_sprite	*img;
	t_vector	*pos;

	query = *ecs_query(ecs, (1ULL << SPRITE) | (1ULL << TRANSFORM));
	while (query.len--)
	{
		img = ecs_entity_get(ecs, query.values[query.len], SPRITE);
		pos = ecs_entity_get(ecs, query.values[query.len], TRANSFORM);
		mlx_put_image_to_window(mlx, win, img->texture, pos->x, pos->y);
	}
}

static void __on_collide(t_ecs *ecs, uint32_t a, uint32_t b)
{
	t_collider	*col1 = ecs_entity_get(ecs, a, COLLIDER);
	t_collider	*col2 = ecs_entity_get(ecs, b, COLLIDER);
	if (col1->type == col2->type)
		return ;
}


int	__on_update(t_app *app, t_scene *scene)
{
	t_env	*env;

	env = (t_env *)scene->env;
	if (__builtin_expect(!env, 0))
		return (0);
	mlx_clear_window(app->mlx, app->win);
	collide_system(env->ecs, &env->grid);
	grid_process(&env->grid, env->ecs, __on_collide);
	draw_system(env->ecs, app->mlx, app->win);
	return (0);
}
