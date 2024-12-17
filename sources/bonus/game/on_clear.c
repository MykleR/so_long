/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_clear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:16:28 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/17 19:27:46 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

int	__game_clear(t_app *app, t_scene *scene)
{
	t_game		*game;

	(void)app;
	game = (t_game *)scene->env;
	grid_destroy(&game->grid);
	ecs_destroy(game->ecs);
	return (0);
}
