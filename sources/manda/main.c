/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:17:59 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/20 21:06:41 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

static int	__app_start(t_app *app)
{
	uint8_t					i;
	t_sprite				*t;
	static const char		*paths[NB_TEXTURES] = {
		"resources/hero/idle_f2.png", "resources/env/floor.png",
		"resources/env/wall_1.png", "resources/env/coin.png",
		"resources/env/exit.png",
	};

	i = -1;
	t = ((t_prog_args *)app->params.args)->textures;
	while (++i < NB_TEXTURES)
	{
		if (open(paths[i], O_RDONLY) < 0)
			return (APP_ERROR);
		t[i].texture = mlx_png_file_to_image(app->mlx,
				(char *)paths[i], &t[i].w, &t[i].h);
	}
	return (0);
}

static int	__app_stop(t_app *app)
{
	uint16_t	i;
	t_sprite	*t;

	i = -1;
	t = ((t_prog_args *)app->params.args)->textures;
	while (++i < NB_TEXTURES)
		if (t[i].texture)
			mlx_destroy_image(app->mlx, t[i].texture);
	return (0);
}

int	main(int ac, char **av)
{
	static t_prog_args	args = {0};
	t_parse_error		status;

	args.argc = ac;
	args.argv = av;
	status = PARSE_ERROR_FILECRASH;
	if (ac == 2)
		status = tilemap_parse(&args.tilemap, av[1]);
	if (status == PARSE_OK)
		status = tilemap_check(&args.tilemap);
	if (status == PARSE_OK)
	{
		app_autorun((t_win_params)
		{__app_start, __app_stop, &args, "so_long", 800, 600, 60}, 1,
			(t_scene){NULL, sizeof(t_env),
			__on_init, __on_event, __on_update, __on_clear});
	}
	else
		print_parse_error(status);
	tilemap_destroy(&args.tilemap);
}
