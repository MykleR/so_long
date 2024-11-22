/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:46:45 by mrouves           #+#    #+#             */
/*   Updated: 2024/11/22 15:02:49 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLICATION_H
# define APPLICATION_H

# include "mlx.h"
# include "mlx_profile.h"
# include <stdint.h>
# include <stdlib.h>
# include <stdbool.h>

# define MAX_SCENES 10

typedef struct s_scene	t_scene;
typedef struct s_app	t_app;
typedef void			(*t_scene_callback)(t_app *, t_scene *);
typedef void			(*t_event_callback)(t_app *, t_scene *, mlx_event_type,
				int);

typedef struct s_win_params
{
	uint32_t			width;
	uint32_t			height;
	uint32_t			fps;
	const char			*name;
}						t_win_params;

typedef struct s_scene
{
	void				*env;
	const char			*name;
	t_scene_callback	on_init;
	t_event_callback	on_event;
	t_scene_callback	on_update;
	t_scene_callback	on_destroy;
}						t_scene;

typedef struct s_app
{
	t_scene				scenes[MAX_SCENES];
	void				*mlx;
	void				*win;
	t_win_params		params;
	uint8_t				scene_last;
	uint8_t				scene_index;
	uint8_t				scene_cap;
	bool				scene_loading;
}						t_app;

void					app_autorun(t_win_params params, t_scene *scenes,
							uint8_t nb_scenes);
void					app_load(t_app *app, uint8_t scene);

int						__app_keyup_hook(int event, void *p);
int						__app_keydown_hook(int event, void *p);
int						__app_mousedown_hook(int event, void *p);
int						__app_mouseup_hook(int event, void *p);
int						__app_mousewheel_hook(int event, void *p);

#endif
