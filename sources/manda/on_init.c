/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:23:51 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 22:45:25 by mrouves          ###   ########.fr       */
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

static uint32_t	tile_create(t_ecs *ecs, t_vector pos,
		t_sprite *sprites, t_tile tile)
{
	uint32_t	id;
	t_sprite	*sprite;

	if (tile == SPAWN)
		tile = FLOOR;
	sprite = sprites + (tile == WALL) + (tile == ITEM) * 2 + (tile == EXIT) * 3;
	id = ecs_entity_create(ecs);
	if (tile == ITEM)
		tile_create(ecs, pos, sprites, FLOOR);
	if (tile == EXIT)
		ecs_entity_add(ecs, id, C_COLLIDER, &((t_collider){NULL,
			sprite->w, sprite->h, T_EXIT}));
	if (tile == ITEM)
		ecs_entity_add(ecs, id, C_COLLIDER, &((t_collider){NULL,
			sprite->w, sprite->h, T_ITEM}));
	ecs_entity_add(ecs, id, C_SPRITE, sprite);
	ecs_entity_add(ecs, id, C_POSITION, &pos);
	return (id);

}

static uint32_t	player_create(t_ecs *ecs, t_vector pos, t_sprite *sprite)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, C_POSITION, &pos);
	ecs_entity_add(ecs, id, C_COLLIDER,
		&((t_collider){__player_collide, sprite->w, sprite->h, T_PLAYER}));
	ecs_entity_add(ecs, id, C_SPRITE, sprite);
	return (id);
}

static void	place_tiles(t_ecs *ecs, t_tilemap *map, t_sprite *sprites)
{
	uint32_t	x;
	uint32_t	y;
	t_vector	pos;
	t_tile		tile;

	y = -1;
	while (++y < map->h)
	{
		x = -1;
		while (++x < map->w)
		{
			pos = (t_vector){x * TILE_SIZE, y * TILE_SIZE};
			tile = tilemap_get(map, y, x);
			tile_create(ecs, pos, sprites + 1, tile);
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
	import_sprite(app->mlx, "resources/hero/idle_f2.png", env->textures);
	import_sprite(app->mlx, "resources/env/tile113.png", env->textures + 1);
	import_sprite(app->mlx, "resources/env/tile005.png", env->textures + 2);
	import_sprite(app->mlx, "resources/coin.png", env->textures + 3);
	import_sprite(app->mlx, "resources/env/tile071.png", env->textures + 4);
	env->player = player_create(env->ecs, (t_vector){
		TILE_SIZE * args->tilemap.spawn.j,
		TILE_SIZE * args->tilemap.spawn.i}, env->textures);
	place_tiles(env->ecs, &args->tilemap, env->textures);
	return (0);
}
