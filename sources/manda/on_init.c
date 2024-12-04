/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:23:51 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 16:18:11 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

static bool	import_sprite(void *mlx, char *path, t_sprite *out)
{
	if (!out)
		return (false);
	out->texture = mlx_png_file_to_image(mlx, path, &out->w, &out->h);
	return (out->texture != NULL);
}

static uint32_t	entity_create(t_ecs *ecs, t_vector pos,
		t_sprite *sprite, t_collider_tag tag)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, C_POSITION, &pos);
	ecs_entity_add(ecs, id, C_COLLIDER,
		&((t_collider){NULL, sprite->w, sprite->h, tag}));
	ecs_entity_add(ecs, id, C_SPRITE, sprite);
	return (id);
}

static void	place_tiles(t_ecs *ecs, t_tilemap *map, t_sprite *sprites)
{
	uint32_t	x;
	uint32_t	y;
	t_tile		tile;

	y = -1;
	while (++y < map->h)
	{
		x = -1;
		while (++x < map->w)
		{
			tile = tilemap_get(map, x, y);
			if (tile == WALL)
				entity_create(ecs, (t_vector){x * TILE_SIZE,
					y * TILE_SIZE}, sprites + 1, T_BLOCK);
			else if (tile == ITEM)
				entity_create(ecs, (t_vector){x * TILE_SIZE,
					y * TILE_SIZE}, sprites + 2, T_ITEM);
		}
	}
}

int	__on_init(t_app *app, t_scene *scene)
{
	t_env		*env;
	t_prog_args	*args;

	env = ft_calloc(sizeof(t_env), 1);
	args = (t_prog_args *)app->params.args;
	if (!env || !args)
		return (APP_ERROR);
	scene->env = env;
	env->camera = (t_aabb){0, 0, app->params.w, app->params.h};
	env->ecs = ecs_create(NB_COMPONENTS, sizeof(t_vector),
			sizeof(t_collider), sizeof(t_sprite));
	if (!env->ecs || !grid_create(&env->grid, env->camera))
		return (APP_ERROR);
	import_sprite(app->mlx, "resources/player.png", env->textures);
	import_sprite(app->mlx, "resources/wall.png", env->textures + 1);
	env->player = entity_create(env->ecs, (t_vector){TILE_SIZE * 8,
			TILE_SIZE * 8}, env->textures, T_PLAYER);
	((t_collider *)(ecs_entity_get(env->ecs, env->player,
						C_COLLIDER)))->on_collide = __player_collide;
	place_tiles(env->ecs, &args->tilemap, env->textures);
	return (0);
}
