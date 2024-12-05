/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:46:45 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/05 18:05:51 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLICATION_H
# define APPLICATION_H

# include <mlx.h>
# include <libft_allocs.h>

# include <stdint.h>
# include <stdbool.h>
# include <stdarg.h>
# include <stdlib.h>

# define MAX_SCENES 5
# define APP_ERROR -1

typedef struct s_scene	t_scene;
typedef struct s_app	t_app;
typedef int				(*t_scene_callback)(t_app *, t_scene *);
typedef int				(*t_event_callback)(t_app *, t_scene *,
		mlx_event_type, int);

typedef struct s_win_params
{
	void				*args;
	const char			*name;
	uint32_t			w;
	uint32_t			h;
	uint32_t			fps;
}						t_win_params;

typedef struct s_scene
{
	void				*env;
	size_t				env_size;			
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

void					app_autorun(t_win_params params,
							uint32_t nb_scenes, ...);
void					app_load(t_app *app, uint8_t scene);

int						__app_kup_hook(int event, void *p);
int						__app_kdown_hook(int event, void *p);
int						__app_mdown_hook(int event, void *p);
int						__app_mup_hook(int event, void *p);
int						__app_mwheel_hook(int event, void *p);
int						__app_window_hook(int event, void *p);

#endif
