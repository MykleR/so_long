/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_start.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 00:12:00 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/22 00:22:52 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

int	__menu_init(t_app *app, t_scene *scene)
{
	(void) scene;
	mlx_set_font_scale(app->mlx, app->win,
		"resources/pixelated.ttf", S_MENU_FONT_S);
	return (0);
}

int	__menu_event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	(void) scene;
	if (t == MLX_MOUSEDOWN && e == 1)
		app_load(app, 1);
	else if (t == MLX_KEYDOWN && e == K_ESCAPE)
		mlx_loop_end(app->mlx);
	return (0);
}

int	__menu_update(t_app *app, t_scene *scene)
{
	(void) scene;
	mlx_clear_window(app->mlx, app->win);
	mlx_string_put(app->mlx, app->win,
		(app->params.w >> 1) - (S_MENU_FONT_S << 2),
		(app->params.h >> 1),
		0xFFFFFFFF, " CLICK     TO     START");
	return (0);
}
