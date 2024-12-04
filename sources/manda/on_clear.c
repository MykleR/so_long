/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_clear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:20:51 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 16:17:56 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

static void	destroy_textures(void *mlx, t_sprite *sprites, uint16_t nb)
{
	uint16_t	i;

	i = -1;
	while (++i < nb)
		if ((sprites + i)->texture)
			mlx_destroy_image(mlx, (sprites + i)->texture);
}

int	__on_clear(t_app *app, t_scene *scene)
{
	t_env		*env;

	if (!scene->env)
		return (0);
	env = (t_env *)scene->env;
	ecs_destroy(env->ecs);
	grid_destroy(&env->grid);
	destroy_textures(app->mlx, env->textures, NB_TEXTURES);
	free(env);
	return (0);
}
