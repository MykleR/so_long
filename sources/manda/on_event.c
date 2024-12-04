/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_event.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:23:23 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 22:43:14 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

static bool	verify_newpos(t_tilemap *map, t_env *env, int x, int y)
{
	bool	success;

	success = tilemap_get(map, y / TILE_SIZE, x / TILE_SIZE) != WALL;
	if (success)
	{
		env->nb_move++;
		ft_printf(1, "%d\n", env->nb_move);
	}
	return (success);
}

int	__on_event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	t_env		*env;
	t_vector	*p;
	t_tilemap	*map;

	env = (t_env *)scene->env;
	map = &((t_prog_args *)(app->params.args))->tilemap;
	p = ecs_entity_get(env->ecs, env->player, C_POSITION);
	if (t == MLX_KEYDOWN && e == K_ESCAPE)
		mlx_loop_end(app->mlx);
	if (t == MLX_KEYDOWN && e == K_LEFT)
		p->x -= TILE_SIZE * verify_newpos(map, env, p->x - TILE_SIZE, p->y);
	if (t == MLX_KEYDOWN && e == K_RIGHT)
		p->x += TILE_SIZE * verify_newpos(map, env, p->x + TILE_SIZE, p->y);
	if (t == MLX_KEYDOWN && e == K_UP)
		p->y -= TILE_SIZE * verify_newpos(map, env, p->x, p->y - TILE_SIZE);
	if (t == MLX_KEYDOWN && e == K_DOWN)
		p->y += TILE_SIZE * verify_newpos(map, env, p->x, p->y + TILE_SIZE);
	return (0);
}
