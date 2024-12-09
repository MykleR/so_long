/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 19:09:21 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/09 16:15:36 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"

static bool	app_scene_init(t_app *app, t_scene *scene)
{
	scene->env = NULL;
	if (scene->env_size)
	{
		scene->env = ft_calloc(scene->env_size, 1);
		if (!scene->env)
			return (false);
	}
	app->scene_loading = scene->on_init
		&& scene->on_init(app, scene) == APP_ERROR;
	return (!app->scene_loading);
}

static bool	app_switch_scene(t_app *app)
{
	t_scene	*n_scene;
	t_scene	*l_scene;
	bool	success;

	n_scene = app->scenes + app->scene_index;
	l_scene = app->scenes + app->scene_last;
	app->scene_last = app->scene_index;
	success = !l_scene->on_destroy
		|| l_scene->on_destroy(app, l_scene) != APP_ERROR;
	free(l_scene->env);
	l_scene->env = NULL;
	return (success && app_scene_init(app, n_scene));
}

static int	__app_update(void *param)
{
	t_app	*app;
	t_scene	*scene;

	app = (t_app *)param;
	if (__builtin_expect(app->scene_loading, 0))
	{
		app->scene_loading = false;
		if (!app_switch_scene(app))
			mlx_loop_end(app->mlx);
		return (0);
	}
	scene = (app->scenes + app->scene_index);
	if (scene->on_update && scene->on_update(app, scene) == APP_ERROR)
		mlx_loop_end(app->mlx);
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
	mlx_loop_hook(app->mlx, __app_update, app);
	app->win = mlx_new_window(app->mlx, params.w, params.h, params.name);
	if (!app->win)
	{
		mlx_destroy_display(app->mlx);
		return (false);
	}
	mlx_on_event(app->mlx, app->win, MLX_KEYUP, __app_kup_hook, app);
	mlx_on_event(app->mlx, app->win, MLX_KEYDOWN, __app_kdown_hook, app);
	mlx_on_event(app->mlx, app->win, MLX_MOUSEUP, __app_mup_hook, app);
	mlx_on_event(app->mlx, app->win, MLX_MOUSEDOWN, __app_mdown_hook, app);
	mlx_on_event(app->mlx, app->win, MLX_MOUSEWHEEL, __app_mwheel_hook, app);
	mlx_on_event(app->mlx, app->win, MLX_WINDOW_EVENT, __app_window_hook, app);
	index = -1;
	while (++index < nb_scenes)
		app->scenes[index] = va_arg(scenes, t_scene);
	return (true);
}

void	app_autorun(t_win_params params, t_app_callback on_start,
			t_app_callback on_stop, uint32_t nb_scenes, ...)
{
	static t_app	app = {0};
	static bool		initialised;
	va_list			scenes;

	if (initialised || !nb_scenes || nb_scenes > MAX_SCENES)
		return ;
	va_start(scenes, nb_scenes);
	initialised = app_init(&app, params, nb_scenes, scenes);
	va_end(scenes);
	if (!initialised)
		return;	
	if (on_start)
		on_start(&app);
	if (app_scene_init(&app, app.scenes))
		mlx_loop(app.mlx);
	if ((app.scenes + app.scene_index)->on_destroy)
		(app.scenes + app.scene_index)->on_destroy(&app,
		(app.scenes + app.scene_index));
	free((app.scenes + app.scene_index)->env);
	if (on_stop)
		on_stop(&app);
	mlx_destroy_window(app.mlx, app.win);
	mlx_destroy_display(app.mlx);
}
