/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 23:29:06 by mykle             #+#    #+#             */
/*   Updated: 2024/12/22 00:17:46 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

int	__game_clear(t_app *app, t_scene *scene)
{
	t_game		*game;

	(void)app;
	game = (t_game *)scene->env;
	grid_destroy(&game->grid);
	ecs_queue_destroy(&game->queue);
	ecs_destroy(game->ecs);
	return (0);
}

#include <stdio.h>
int	__game_update(t_app *app, t_scene *scene)
{
	t_prog_args	*args;
	t_game		*game;

	game = (t_game *)scene->env;
	args = (t_prog_args *)app->params.args;
	if (game->spawn_cooldown++ >= S_ENEMY_SPAWNRATE)
	{
		game->spawn_cooldown = 0;
		instantiate_enemy(game->ecs, args->imgs_hero + 1, (t_vector){
			rand() % game->world.w, rand() % game->world.h});
	}
	game_physics(app, game);
	game_render(app, game);
	return (0);
}

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

	game = scene->env;
	args = app->params.args;
	if (!game || !args)
		return (APP_ERROR);
	srand(time(NULL));
	game->to_collect = args->tilemap.to_collect;
	game->camera = (t_aabb){0, 0, app->params.w, app->params.h};
	game->world = (t_aabb){0, 0, args->tilemap.size.j * TILE_SIZE,
		args->tilemap.size.i * TILE_SIZE};
	game->ecs = ecs_create(N_COMPS,
			sizeof(t_vector), sizeof(t_vector), sizeof(t_vector),
			sizeof(t_collider), sizeof(t_sprite), sizeof(t_animation),
			sizeof(uint32_t), sizeof(uint32_t), sizeof(int32_t));
	if (!game->ecs || !grid_create(&game->grid, game->world)
		|| !ecs_queue_create(&game->queue))
		return (APP_ERROR);
	game->player = instantiate_player(game->ecs, *args->imgs_hero,
			args->tilemap.spawn.j * TILE_SIZE,
			args->tilemap.spawn.i * TILE_SIZE);
	place_tiles(game->ecs, args->tilemap, args->imgs_env + 1);
	return (0);
}
