/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:23:51 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 23:19:20 by mrouves          ###   ########.fr       */
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

static uint32_t tile_create(t_ecs *ecs, int x, int y, t_sprite s)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, TRANSFORM, &((t_vector){x, y}));
	ecs_entity_add(ecs, id, COLLIDER, &((t_collider){s.w, s.h, STATIC}));
	ecs_entity_add(ecs, id, SPRITE, &s);
	return (id);
}

static void	place_tiles(t_ecs *ecs, t_tilemap *map, t_sprite *sprites)
{
	uint32_t	x;
	uint32_t	y;

	y = -1;
	while (++y < map->h)
	{
		x = -1;
		while (++x < map->w)
			if (tilemap_get(map, x, y) == WALL)
				tile_create(ecs, x * SPRITE_SIZE, y * SPRITE_SIZE, sprites[1]);
	}
}

int	__on_init(t_app *app, t_scene *scene)
{
	t_env		*env;
	t_prog_args *args;

	env = ft_calloc(sizeof(t_env), 1);
	args = (t_prog_args *)app->params.args;
	if (!env || !args)
		return (APP_ERROR);
	scene->env = env;
	env->ecs = ecs_create(NB_COMPONENTS, sizeof(t_vector),
			sizeof(t_collider), sizeof(t_sprite));
	if (!env->ecs)
		return (APP_ERROR);
	env->camera = (t_aabb){0, 0, app->params.w, app->params.h};
	if (!grid_create(&env->grid, env->camera))
		return (APP_ERROR);
	import_sprite(app->mlx, "resources/player.png", env->textures);
	import_sprite(app->mlx, "resources/wall.png", env->textures + 1);
	place_tiles(env->ecs, &args->tilemap, env->textures);
	env->player = player_create(env->ecs, SPRITE_SIZE * 15,
					SPRITE_SIZE * 15, env->textures[0]);
	env->last_pos = (t_vector){app->params.w / 2, app->params.h /2};
	return (0);
}
