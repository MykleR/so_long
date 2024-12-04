/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:17:59 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 23:19:31 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

//TODO CHECKPARSING
int	main(int ac, char **av)
{
	static t_prog_args	args = {0};

	args.argc = ac;
	args.argv = av;
	if (ac == 2 && tilemap_parse(&args.tilemap, av[1])
		&& tilemap_check(&args.tilemap))
	{
		app_autorun((t_win_params){&args, "so_long", 800, 600, 60}, 1,
			(t_scene){NULL, __on_init, __on_event, __on_update, __on_clear});
	}
	tilemap_destroy(&args.tilemap);
}
