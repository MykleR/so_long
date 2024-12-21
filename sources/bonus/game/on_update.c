/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:20:21 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/21 16:12:02 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

int	__game_update(t_app *app, t_scene *scene)
{
	t_game		*game;

	game = (t_game *)scene->env;
	game_physics(app, game);
	game_render(app, game);
	return (0);
}
