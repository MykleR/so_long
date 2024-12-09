/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_clear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:16:28 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/09 16:48:30 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

int	__game_clear(t_app *app, t_scene *scene)
{
	t_game		*game;

	(void)app;
	game = (t_game *)scene->env;
	ecs_destroy(game->ecs);
	grid_destroy(&game->grid);
	return (0);
}
