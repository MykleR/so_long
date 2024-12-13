/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:30:01 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/13 20:24:55 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static int	__app_start(t_app *app)
{
	uint8_t					i;
	t_sprite				*t;
	static const char		*paths[NB_TEXTURES] = {
		"resources/hero/idle_f2.png",
	};

	i = -1;
	t = ((t_prog_args *)app->params.args)->sprites;
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
	t = ((t_prog_args *)app->params.args)->sprites;
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
		app_autorun((t_win_params){
			__app_start, __app_stop, &args, "so_long", 800, 600, 60}, NB_SCENES,
			(t_scene){NULL, 0,
			__menu_init, __menu_event, __menu_update, __menu_clear},
			(t_scene){NULL, sizeof(t_game),
			__game_init, __game_event, __game_update, __game_clear});
	}
	else
		print_parse_error(status);
	tilemap_destroy(&args.tilemap);
}
