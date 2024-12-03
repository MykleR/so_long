/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:17:59 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 15:03:49 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manda.h"

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	app_autorun((t_win_params){"so_long", 800, 600, 60}, 1,
		(t_scene){NULL, __on_init, __on_event, __on_update, __on_clear});
}
