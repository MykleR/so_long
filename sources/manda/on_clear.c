/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_clear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:20:51 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/04 13:26:52 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

int	__on_clear(t_app *app, t_scene *scene)
{
	t_env		*env;
	uint32_t	i;

	if (!scene->env)
		return (0);
	env = (t_env *)scene->env;
	ecs_destroy(env->ecs);
	grid_destroy(&env->grid);
	i = -1;
	while (++i < NB_TEXTURES)
		if (env->textures[i].texture)
			mlx_destroy_image(app->mlx, env->textures[i].texture);
	free(env);
	return (0);
}
