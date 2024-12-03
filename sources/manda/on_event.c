/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_event.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:23:23 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 16:23:48 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manda.h"

int	__on_event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	(void) scene;
	if (t == MLX_KEYDOWN && e == K_ESCAPE)
		mlx_loop_end(app->mlx);
	return (0);
}
