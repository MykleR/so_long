/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 19:09:21 by mrouves           #+#    #+#             */
/*   Updated: 2024/11/21 18:47:52 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"
#include "libft.h"

static int	__app_update(void *param)
{
	t_app	*app;
	t_scene	*scene;

	app = (t_app *)param;
	scene = (app->scenes + app->scene_index);
	if (scene->on_update)
		scene->on_update(app, scene);
	if (app->scene_last != app->scene_index)
	{
		scene = app->scenes + app->scene_last;
		if (scene->on_destroy)
			scene->on_destroy(app, scene);
		scene = app->scenes + app->scene_index;
		if (scene->on_init)
			scene->on_init(app, scene);
		app->scene_last = app->scene_index;
	}
	return (0);
}

static int	__app_hook(int event, void *param)
{
	if (event == 0)
		mlx_loop_end(((t_app *)param)->mlx);
	return (0);
}

void	app_load(t_app *app, uint8_t index)
{
	if (__builtin_expect(!app || index >= app->scene_cap, 0))
		return ;
	app->scene_last = app->scene_index;
	app->scene_index = index;
}

void	app_autorun(t_win_params params, t_scene *scenes, uint8_t nb_scenes)
{
	static t_app	app = {0};

	if (app.mlx || app.win || !nb_scenes || ! scenes || nb_scenes > MAX_SCENES)
		return ;
	app.params = params;
	app.scene_cap = nb_scenes;
	app.mlx = mlx_init();
	if (!app.mlx)
		return ;
	app.win = mlx_new_window(app.mlx, params.width, params.height, params.name);
	if (!app.win)
		return ;
	ft_memset(app.scenes, 0, sizeof(t_scene) * MAX_SCENES);
	ft_memcpy(app.scenes, scenes, sizeof(t_scene) * nb_scenes);
	mlx_set_fps_goal(app.mlx, params.fps);
	mlx_on_event(app.mlx, app.win, MLX_WINDOW_EVENT, __app_hook, &app);
	if (app.scenes->on_init)
		app.scenes->on_init(&app, app.scenes);
	mlx_loop_hook(app.mlx, __app_update, &app);
	mlx_loop(app.mlx);
	if (app.scenes[app.scene_index].on_destroy)
		app.scenes[app.scene_index].on_destroy(&app, app.scenes + app.scene_index);
	mlx_destroy_window(app.mlx, app.win);
	mlx_destroy_display(app.mlx);
}
