/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_event.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:23:23 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 13:26:58 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

int	__on_event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	t_env		*env;
	t_vector	*pos;

	env = (t_env *)scene->env;
	pos = ecs_entity_get(env->ecs, env->player, TRANSFORM);
	env->last_pos = *pos;
	if (t == MLX_KEYDOWN && e == K_ESCAPE)
		mlx_loop_end(app->mlx);
	if (t == MLX_KEYDOWN && e == K_LEFT)
		pos->x -= SPRITE_SIZE;
	if (t == MLX_KEYDOWN && e == K_RIGHT)
		pos->x += SPRITE_SIZE;
	if (t == MLX_KEYDOWN && e == K_UP)
		pos->y -= SPRITE_SIZE;
	if (t == MLX_KEYDOWN && e == K_DOWN)
		pos->y += SPRITE_SIZE;
	return (0);
}
