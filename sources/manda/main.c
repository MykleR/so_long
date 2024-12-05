/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:17:59 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/05 13:08:21 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

int	main(int ac, char **av)
{
	static t_prog_args	args = {0};
	t_parse_error		status;

	args.argc = ac;
	args.argv = av;
	status = tilemap_parse(&args.tilemap, av[1]);
	if (status == PARSE_OK)
		status = tilemap_check(&args.tilemap);
	if (status == PARSE_OK)
		app_autorun((t_win_params){&args, "so_long", 800, 600, 60}, 1,
			(t_scene){NULL, __on_init, __on_event, __on_update, __on_clear});
	if (status != PARSE_OK)
		print_parse_error(status);
	tilemap_destroy(&args.tilemap);
}
