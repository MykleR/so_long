/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 19:09:21 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 13:26:18 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"

static int	__app_stop(t_app *app, bool error)
{
	app->app_error = error;
	mlx_loop_end(app->mlx);
	return (error);
}

static int	__app_update(void *param)
{
	t_app	*app;
	t_scene	*scene;

	app = (t_app *)param;
	scene = (app->scenes + app->scene_index);
	if (scene->on_update && scene->on_update(app, scene) == APP_ERROR)
		return (__app_stop(app, 1));
	if (__builtin_expect(!app->scene_loading, 1))
		return (0);
	scene = app->scenes + app->scene_last;
	if (scene->on_destroy && scene->on_destroy(app, scene) == APP_ERROR)
		return (__app_stop(app, 1));
	scene->env = 0;
	scene = app->scenes + app->scene_index;
	if (scene->on_init && scene->on_init(app, scene) == APP_ERROR)
		return (__app_stop(app, 1));
	app->scene_last = app->scene_index;
	app->scene_loading = false;
	return (0);
}

static bool	app_init(t_app *app, t_win_params params,
				uint8_t nb_scenes, va_list scenes)
{
	uint8_t	index;

	app->params = params;
	app->scene_cap = nb_scenes;
	app->mlx = mlx_init();
	if (!app->mlx)
		return (false);
	mlx_set_fps_goal(app->mlx, params.fps);
	app->win = mlx_new_window(app->mlx, params.w, params.h, params.name);
	if (!app->win)
		return (false);
	mlx_on_event(app->mlx, app->win, MLX_KEYUP, __app_kup_hook, app);
	mlx_on_event(app->mlx, app->win, MLX_KEYDOWN, __app_kdown_hook, app);
	mlx_on_event(app->mlx, app->win, MLX_MOUSEUP, __app_mup_hook, app);
	mlx_on_event(app->mlx, app->win, MLX_MOUSEDOWN, __app_mdown_hook, app);
	mlx_on_event(app->mlx, app->win, MLX_MOUSEWHEEL, __app_mwheel_hook, app);
	mlx_on_event(app->mlx, app->win, MLX_WINDOW_EVENT, __app_window_hook, app);
	mlx_loop_hook(app->mlx, __app_update, app);
	index = -1;
	while (++index < nb_scenes)
		app->scenes[index] = va_arg(scenes, t_scene);
	return (true);
}

void	app_autorun(t_win_params params, uint32_t nb_scenes, ...)
{
	static t_app	app = {0};
	va_list			scenes;
	t_scene			*scene;

	if (app.mlx || app.win || !nb_scenes || nb_scenes > MAX_SCENES)
		return ;
	va_start(scenes, nb_scenes);
	if (app_init(&app, params, nb_scenes, scenes))
	{
		scene = app.scenes;
		if (!scene->on_init || scene->on_init(&app, scene) != APP_ERROR)
			mlx_loop(app.mlx);
		scene = app.scenes + app.scene_index;
		if (scene->on_destroy)
			scene->on_destroy(&app, scene);
	}
	va_end(scenes);
	if (app.mlx && app.win)
		mlx_destroy_window(app.mlx, app.win);
	if (app.mlx)
		mlx_destroy_display(app.mlx);
}
