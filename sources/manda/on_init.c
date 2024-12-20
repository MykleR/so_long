/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:23:51 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/09 16:21:07 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

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

static uint32_t	player_create(t_ecs *ecs, t_sprite *sprite, t_vector pos)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, C_POSITION, &pos);
	ecs_entity_add(ecs, id, C_COLLIDER,
		&((t_collider){__player_collide, sprite->w, sprite->h, T_PLAYER}));
	ecs_entity_add(ecs, id, C_SPRITE, sprite);
	return (id);
}

static void	place_tiles(t_ecs *ecs, t_tilemap map, t_sprite *sprites)
{
	uint32_t	i;
	uint32_t	j;
	t_vector	pos;
	t_tile		tile;

	i = -1;
	while (++i < map.size.i)
	{
		j = -1;
		while (++j < map.size.j)
		{
			pos = (t_vector){j * TILE_SIZE, i * TILE_SIZE};
			tile = tilemap_get(map, i, j);
			tile_create(ecs, pos, sprites + 1, tile);
		}
	}
}

int	__on_init(t_app *app, t_scene *scene)
{
	t_env		*env;
	t_prog_args	*args;

	env = (t_env *)scene->env;
	args = (t_prog_args *)app->params.args;
	if (!env || !args)
		return (APP_ERROR);
	env->camera = (t_aabb){
		TILE_SIZE * (args->tilemap.spawn.j - 1) - app->params.w / 2,
		TILE_SIZE * (args->tilemap.spawn.i - 1) - app->params.h / 2,
		app->params.w, app->params.h};
	env->ecs = ecs_create(NB_COMPONENTS, sizeof(t_vector),
			sizeof(t_collider), sizeof(t_sprite));
	if (!env->ecs || !grid_create(&env->grid, env->camera))
		return (APP_ERROR);
	env->player = player_create(env->ecs, args->textures, (t_vector){
			TILE_SIZE * args->tilemap.spawn.j,
			TILE_SIZE * args->tilemap.spawn.i});
	place_tiles(env->ecs, args->tilemap, args->textures);
	env->to_collect = args->tilemap.to_collect;
	return (0);
}
