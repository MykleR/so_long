/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_death.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:33:02 by mykle             #+#    #+#             */
/*   Updated: 2024/12/21 18:33:06 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

int	__death_init(t_app *app, t_scene *scene)
{
	t_menu_death	*menu;

	menu = scene->env;
	menu->count = 0;
	menu->frame = 5;
	mlx_set_font_scale(app->mlx, app->win,
		"resources/pixelated.ttf", S_MENU_FONT_S);
	return (0);
}

int	__death_event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	t_menu_death	*menu;

	menu = scene->env;
	if (t == MLX_MOUSEDOWN && e == 1 && menu->frame == N_IMGS_HERO - 1)
		app_load(app, 1);
	else if (t == MLX_KEYDOWN && e == K_ESCAPE)
		mlx_loop_end(app->mlx);
	return (0);
}

int	__death_update(t_app *app, t_scene *scene)
{
	t_prog_args		*args;
	t_menu_death	*menu;

	args = app->params.args;
	menu = scene->env;
	menu->count += menu->frame < N_IMGS_HERO - 1;
	if (menu->count >= 5)
	{
		menu->count = 0;
		menu->frame++;
	}
	mlx_clear_window(app->mlx, app->win);
	if (menu->frame >= N_IMGS_HERO - 1)
		mlx_string_put(app->mlx, app->win,
			(app->params.w >> 1) - (S_MENU_FONT_S << 1),
			(app->params.h >> 1), 0xFFFFFFFF, "YOU DIED");
	else
		mlx_put_image_to_window(app->mlx, app->win,
				args->imgs_hero[menu->frame].texture,
				(app->params.w >> 1) - 128,
				(app->params.h >> 1) - 128);
	return (0);
}
