/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:20:21 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/19 20:46:36 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

int	__game_update(t_app *app, t_scene *scene)
{
	t_game		*game;

	game = (t_game *)scene->env;
	game_physics(scene, game);
	game_render(app, game);
	return (0);
}
