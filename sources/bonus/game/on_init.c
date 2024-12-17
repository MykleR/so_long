/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:20:10 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/17 21:14:44 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static uint32_t	player_create(t_ecs *ecs, t_sprite sprite, float x, float y)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, C_POSITION, &((t_vector){x, y}));
	ecs_entity_add(ecs, id, C_SPRITE, &sprite);
	ecs_entity_add(ecs, id, C_VELOCITY, &((t_vector){0, 0}));
	ecs_entity_add(ecs, id, C_COLLIDER, &((t_collider){__player_collide,
			sprite.w, sprite.h, T_PLAYER}));
	return (id);
}

static void	tile_create(t_ecs *ecs, t_vector pos,
		t_sprite *sprites, t_tile tile)
{
	uint32_t	id;

	if (tile != WALL)
		return ;
	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, C_SPRITE, sprites);
	ecs_entity_add(ecs, id, C_POSITION, &pos);
	ecs_entity_add(ecs, id, C_COLLIDER, &((t_collider){NULL,
		sprites->w, sprites->h, T_BLOCK}));
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
			tile_create(ecs, pos, sprites, tile);
		}
	}
}

int	__game_init(t_app *app, t_scene *scene)
{
	t_game		*game;
	t_prog_args	*args;

	game = (t_game *)scene->env;
	args = (t_prog_args *)app->params.args;
	if (!game || !args)
		return (APP_ERROR);
	game->camera = (t_aabb){0, 0, app->params.w, app->params.h};
	game->ecs = ecs_create(NB_COMPS, sizeof(t_vector), sizeof(t_vector),
			sizeof(t_collider), sizeof(t_sprite), sizeof(uint32_t),
			sizeof(uint32_t), sizeof(uint32_t));
	if (!game->ecs || !grid_create(&game->grid, game->camera))
		return (APP_ERROR);
	game->player = player_create(game->ecs, *args->imgs_hero,
			args->tilemap.spawn.j * TILE_SIZE,
			args->tilemap.spawn.i * TILE_SIZE);
	game->gravity = (t_vector){0, 0.6};
	place_tiles(game->ecs, args->tilemap, args->imgs_env);
	return (0);
}
