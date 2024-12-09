/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:17:59 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/09 16:20:28 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

static void	__app_start(t_app *app)
{
	uint8_t			i;
	t_sprite		*t;
	static const char		*paths[NB_TEXTURES] = {
		"resources/hero/idle_f2.png", "resources/env/tile113.png",
		"resources/env/tile005.png", "resources/coin.png",
		"resources/env/tile071.png",
	};

	i = -1;
	t = ((t_prog_args *)app->params.args)->textures;
	while (++i < NB_TEXTURES)
		t[i].texture = mlx_png_file_to_image(app->mlx,
			(char *)paths[i], &t[i].w, &t[i].h);
}
static void	__app_stop(t_app *app)
{
	uint16_t	i;
	t_sprite	*t;


	i = -1;
	t = ((t_prog_args *)app->params.args)->textures;
	while (++i < NB_TEXTURES)
		if (t[i].texture)
			mlx_destroy_image(app->mlx, t[i].texture);
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
		app_autorun((t_win_params){&args, "so_long", 800, 600, 60},
			__app_start, __app_stop, 1,
			(t_scene){NULL, sizeof(t_env),
			__on_init, __on_event, __on_update, __on_clear});
	}
	else
		print_parse_error(status);
	tilemap_destroy(&args.tilemap);
}
