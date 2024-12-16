/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_event.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:16:41 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/16 22:41:55 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

void	__bullet_collide(uint32_t self, uint32_t other, void *data)
{
	t_game		*game;
	t_collider	*other_col;

	game = (t_game *)((t_scene *)data)->env;
	other_col = ecs_entity_get(game->ecs, other, C_COLLIDER);
	if (other_col->tag == T_BLOCK)
		ecs_entity_kill(game->ecs, self);
}

static void create_bullet(t_app *app, t_game *game, t_vector pos, t_vector vel)
{
	uint32_t	id;
	t_prog_args	*args;

	args = (t_prog_args *)app->params.args;
	id = ecs_entity_create(game->ecs);
	ecs_entity_add(game->ecs, id, C_POSITION, &pos);
	ecs_entity_add(game->ecs, id, C_VELOCITY, &vel);
	ecs_entity_add(game->ecs, id, C_SPRITE, args->sprites + 1);
	ecs_entity_add(game->ecs, id, C_COLLIDER, &((t_collider){__bullet_collide,
		args->sprites[1].w, args->sprites[1].h, T_PROJECTILE}));
}

static void shotgun(t_app *app, t_game *game, t_vector pos, float angle)
{
	uint32_t	i;
	float		fov;
	float		current_angle;

	i = -1;
	fov = PLAYER_SHOOT_FOV * M_PI / 180;
	current_angle = angle - fov * .5;
	while (++i < PLAYER_SHOOT_NB)
	{
		create_bullet(app, game, pos, (t_vector){
			cos(current_angle) * BULLET_SHOOT_F,
			sin(current_angle) * BULLET_SHOOT_F
		});
		current_angle += fov / PLAYER_SHOOT_NB;
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
	vel = ecs_entity_get(game->ecs, game->player, C_VELOCITY);
	pos = ecs_entity_get(game->ecs, game->player, C_POSITION);
	angle = atan2(my + game->camera.y - pos->y, mx + game->camera.x - pos->x);
	vel->x = cos(angle) * -PLAYER_JUMP_F;
	vel->y = sin(angle) * -PLAYER_JUMP_F;
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
