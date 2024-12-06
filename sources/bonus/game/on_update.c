/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:20:21 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/06 16:49:03 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

int	__game_update(t_app *app, t_scene *scene)
{
	(void)scene;
	mlx_clear_window(app->mlx, app->win);
	return (0);
}
