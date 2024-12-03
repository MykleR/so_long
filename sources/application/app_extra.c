/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_extra.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:33:40 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 12:34:53 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"

void	app_load(t_app *app, uint8_t index)
{
	if (__builtin_expect(!app || index >= app->scene_cap
			|| app->scene_loading, 0))
		return ;
	app->scene_last = app->scene_index;
	app->scene_index = index;
	app->scene_loading = true;
}

int	__app_window_hook(int event, void *p)
{
	t_app	*app;
	t_scene	*scene;

	app = (t_app *)p;
	scene = (app->scenes + app->scene_index);
	if (__builtin_expect(!event, 0))
		mlx_loop_end(app->mlx);
	if (__builtin_expect(scene->on_event && !app->scene_loading, 1))
		scene->on_event(app, scene, MLX_WINDOW_EVENT, event);
	return (0);
}
