/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:20:10 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/19 23:17:31 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static void	place_tiles(t_ecs *ecs, t_tilemap map, t_sprite *sprites)
{
	t_vector	tile_pos;
	t_vector	world_pos;
	t_tile		tile;

	tile_pos = (t_vector){-1, -1};
	while (++tile_pos.y < map.size.i)
	{
		tile_pos.x = -1;
		while (++tile_pos.x < map.size.j)
		{
			world_pos = (t_vector){tile_pos.x * TILE_SIZE,
				tile_pos.y * TILE_SIZE};
			tile = tilemap_get(map, tile_pos.y, tile_pos.x);
			instantiate_tile(ecs, sprites, world_pos, tile);
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
	game->ecs = ecs_create(N_COMPS, sizeof(t_vector), sizeof(t_vector),
			sizeof(t_vector), sizeof(t_collider), sizeof(t_sprite),
			sizeof(t_animation), sizeof(uint32_t),
			sizeof(uint32_t), sizeof(uint32_t));
	if (!game->ecs || !grid_create(&game->grid, game->camera)
		|| !ecs_queue_create(&game->queue))
		return (APP_ERROR);
	game->player = instantiate_player(game->ecs, *args->imgs_hero,
			args->tilemap.spawn.j * TILE_SIZE,
			args->tilemap.spawn.i * TILE_SIZE);
	uint32_t test = ecs_entity_create(game->ecs);
	ecs_entity_add(game->ecs, test, COMP_POS, &((t_vector){100,100}));
	ecs_entity_add(game->ecs, test, COMP_IMG, args->imgs_hero);
	ecs_entity_add(game->ecs, test, COMP_ANIM, &((t_animation){args->imgs_hero, N_IMGS_HERO, 0, 60, 0, 1}));
	place_tiles(game->ecs, args->tilemap, args->imgs_env);
	return (0);
}
