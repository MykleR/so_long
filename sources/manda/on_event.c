/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_event.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:23:23 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 16:14:11 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

static bool	verify_newpos(t_tilemap *map, int x, int y)
{
	return (tilemap_get(map, x / TILE_SIZE, y / TILE_SIZE) != WALL);
}

int	__on_event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	t_env		*env;
	t_vector	*pos;
	t_tilemap	*map;

	env = (t_env *)scene->env;
	map = &((t_prog_args *)(app->params.args))->tilemap;
	pos = ecs_entity_get(env->ecs, env->player, C_POSITION);
	if (t == MLX_KEYDOWN && e == K_ESCAPE)
		mlx_loop_end(app->mlx);
	if (t == MLX_KEYDOWN && e == K_LEFT)
		pos->x -= TILE_SIZE * verify_newpos(map, pos->x - TILE_SIZE, pos->y);
	if (t == MLX_KEYDOWN && e == K_RIGHT)
		pos->x += TILE_SIZE * verify_newpos(map, pos->x + TILE_SIZE, pos->y);
	if (t == MLX_KEYDOWN && e == K_UP)
		pos->y -= TILE_SIZE * verify_newpos(map, pos->x, pos->y - TILE_SIZE);
	if (t == MLX_KEYDOWN && e == K_DOWN)
		pos->y += TILE_SIZE * verify_newpos(map, pos->x, pos->y + TILE_SIZE);
	return (0);
}
