/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_event.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:16:41 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/21 16:46:51 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static void	shotgun(t_app *app, t_game *game, t_vector pos, float angle)
{
	uint32_t	i;
	float		fov;
	float		current_angle;
	t_sprite	*imgs;

	imgs = ((t_prog_args *)app->params.args)->imgs_fx;
	fov = S_PLAYER_SHOOT_FOV * M_PI / 180;
	current_angle = angle;
	if (game->collected > 0)
		current_angle = angle - fov * .5;
	i = -1;
	instantiate_particule(game->ecs, (t_animation){imgs + 4, 5, 0, 4, 0},
		(t_vector){pos.x - (float)imgs->w, pos.y - (float)imgs->h}, 10);
	while (++i < game->collected + 1)
	{
		instantiate_pbullet(game->ecs, imgs, pos, (t_vector){
			cos(current_angle) * S_BULLET_SHOOT_F,
			sin(current_angle) * S_BULLET_SHOOT_F});
		current_angle += fov / (game->collected + 1);
	}
}

static void	player_shoot(t_app *app, t_game *game)
{
	t_vector	*vel;
	t_vector	*pos;
	double		angle;
	int			mx;
	int			my;

	mlx_mouse_get_pos(app->mlx, &mx, &my);
	vel = ecs_entity_get(game->ecs, game->player, COMP_VEL);
	pos = ecs_entity_get(game->ecs, game->player, COMP_POS);
	angle = atan2(my + game->camera.y - pos->y, mx + game->camera.x - pos->x);
	vel->x = cos(angle) * -S_PLAYER_SHOOT_F;
	vel->y = sin(angle) * -S_PLAYER_SHOOT_F;
	shotgun(app, game, *pos, angle);
}

int	__game_event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	(void) scene;
	if (t == MLX_KEYDOWN && e == K_ESCAPE)
		app_load(app, 0);
	if (t == MLX_MOUSEDOWN && e == 1)
		player_shoot(app, (t_game *)scene->env);
	return (0);
}
