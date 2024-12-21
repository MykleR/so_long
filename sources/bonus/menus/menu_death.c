/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_death.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:33:02 by mykle             #+#    #+#             */
/*   Updated: 2024/12/21 17:37:20 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

int	__death_init(t_app *app, t_scene *scene)
{
	(void) scene;
	mlx_set_font_scale(app->mlx, app->win,
		"resources/pixelated.ttf", S_MENU_FONT_S);
	return (0);
}

int	__death_event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	(void) scene;
	if (t == MLX_MOUSEDOWN && e == 1)
		app_load(app, 1);
	else if (t == MLX_KEYDOWN && e == K_ESCAPE)
		mlx_loop_end(app->mlx);
	return (0);
}

int	__death_update(t_app *app, t_scene *scene)
{
	(void) scene;
	mlx_clear_window(app->mlx, app->win);
	mlx_string_put(app->mlx, app->win,
		(app->params.w >> 1) - (S_MENU_FONT_S << 1),
		(app->params.h >> 1),
		0xFFFFFFFF, "YOU DIED");
	return (0);
}
