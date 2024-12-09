/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_clear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:20:51 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/09 16:21:27 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <manda.h>

int	__on_clear(t_app *app, t_scene *scene)
{
	t_env		*env;

	(void) app;
	env = (t_env *)scene->env;
	ecs_destroy(env->ecs);
	grid_destroy(&env->grid);
	return (0);
}
