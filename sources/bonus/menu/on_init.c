/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 00:12:00 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/20 21:05:32 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

int	__menu_init(t_app *app, t_scene *scene)
{
	(void) scene;
	mlx_set_font_scale(app->mlx, app->win,
		"resources/pixelated.ttf", S_MENU_FONT_S);
	return (0);
}
