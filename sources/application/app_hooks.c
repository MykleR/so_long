/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 12:40:33 by mrouves           #+#    #+#             */
/*   Updated: 2024/11/22 14:54:43 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"

int	__app_keyup_hook(int event, void *p)
{
	t_app	*app;
	t_scene	*scene;

	app = (t_app *)p;
	scene = (app->scenes + app->scene_index);
	if (__builtin_expect(scene->on_event != NULL && !app->scene_loading, 1))
		scene->on_event(app, scene, MLX_KEYUP, event);
	return (0);
}

int	__app_keydown_hook(int event, void *p)
{
	t_app	*app;
	t_scene	*scene;

	app = (t_app *)p;
	scene = (app->scenes + app->scene_index);
	if (__builtin_expect(scene->on_event != NULL && !app->scene_loading, 1))
		scene->on_event(app, scene, MLX_KEYDOWN, event);
	return (0);
}

int	__app_mousedown_hook(int event, void *p)
{
	t_app	*app;
	t_scene	*scene;

	app = (t_app *)p;
	scene = (app->scenes + app->scene_index);
	if (__builtin_expect(scene->on_event != NULL && !app->scene_loading, 1))
		scene->on_event(app, scene, MLX_MOUSEDOWN, event);
	return (0);
}

int	__app_mouseup_hook(int event, void *p)
{
	t_app	*app;
	t_scene	*scene;

	app = (t_app *)p;
	scene = (app->scenes + app->scene_index);
	if (__builtin_expect(scene->on_event != NULL && !app->scene_loading, 1))
		scene->on_event(app, scene, MLX_MOUSEUP, event);
	return (0);
}

int	__app_mousewheel_hook(int event, void *p)
{
	t_app	*app;
	t_scene	*scene;

	app = (t_app *)p;
	scene = (app->scenes + app->scene_index);
	if (__builtin_expect(scene->on_event != NULL && !app->scene_loading, 1))
		scene->on_event(app, scene, MLX_MOUSEWHEEL, event);
	return (0);
}
