/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_clear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:16:28 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/06 18:02:58 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static void	destroy_textures(void *mlx, t_sprite *sprites, uint16_t nb)
{
	uint16_t	i;

	i = -1;
	while (++i < nb)
		if ((sprites + i)->texture)
			mlx_destroy_image(mlx, (sprites + i)->texture);
}

int	__game_clear(t_app *app, t_scene *scene)
{
	t_game		*game;

	game = (t_game *)scene->env;
	ecs_destroy(game->ecs);
	grid_destroy(&game->grid);
	destroy_textures(app->mlx, game->sprites, NB_TEXTURES);
	return (0);
}
