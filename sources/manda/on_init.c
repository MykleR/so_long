/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:23:51 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 16:08:41 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manda.h"

static bool	import_sprite(void *mlx, char *path, t_sprite *out)
{
	if (!out)
		return (false);
	out->texture = mlx_png_file_to_image(mlx, path, &out->w, &out->h);
	return (out->texture != NULL);
}

static uint32_t	player_create(t_ecs *ecs, int x, int y, t_sprite s)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, TRANSFORM, &((t_vector){x, y}));
	ecs_entity_add(ecs, id, COLLIDER, &((t_collider){s.w, s.h, PLAYER}));
	ecs_entity_add(ecs, id, SPRITE, &s);
	return (id);
}

int	__on_init(t_app *app, t_scene *scene)
{
	t_env	*env;

	env = ft_calloc(sizeof(t_env), 1);
	if (!env)
		return (APP_ERROR);
	scene->env = env;
	env->ecs = ecs_create(NB_COMPONENTS, sizeof(t_vector),
			sizeof(t_collider), sizeof(t_sprite));
	if (!env->ecs)
		return (APP_ERROR);
	if (!grid_create(&env->grid, (t_aabb){0, 0, app->params.w, app->params.h}))
		return (APP_ERROR);
	import_sprite(app->mlx, "resources/player.png", env->textures);
	import_sprite(app->mlx, "resources/wall.png", env->textures + 1);
	env->player = player_create(env->ecs, 100, 100, env->textures[0]);
	return (0);
}
