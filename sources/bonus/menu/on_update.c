/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 00:14:22 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/06 15:00:46 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <bonus.h>

int	__menu_update(t_app *app, t_scene *scene)
{
	(void) scene;
	mlx_string_put(app->mlx, app->win,
		(app->params.w >> 1) - (MENU_FONT_SIZE << 1),
		(app->params.h >> 1) - (MENU_FONT_SIZE << 1),
		0xFFFFFFFF, "MAIN MENU");
	return (0);
}
