/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:30:01 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/19 21:09:23 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static int	__app_start(t_app *app)
{
	return (load_app_resources(app->mlx, app->params.args));
}

static int	__app_stop(t_app *app)
{
	unload_app_resources(app->mlx, app->params.args);
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
			__app_start, __app_stop, &args, "so_long", 800, 600, 60}, N_SCENES,
			(t_scene){NULL, 0,
			__menu_init, __menu_event, __menu_update, __menu_clear},
			(t_scene){NULL, sizeof(t_game),
			__game_init, __game_event, __game_update, __game_clear});
	}
	else
		print_parse_error(status);
	tilemap_destroy(&args.tilemap);
}
