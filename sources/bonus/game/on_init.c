/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:20:10 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/06 18:01:36 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static uint32_t	player_create(t_ecs *ecs, t_sprite sprite, float x, float y)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, C_POSITION, &((t_vector){x ,y}));
	ecs_entity_add(ecs, id, C_SPRITE, &sprite);
	ecs_entity_add(ecs, id, C_VELOCITY, &((t_vector){0, 0}));
	ecs_entity_add(ecs, id, C_COLLIDER, &((t_collider){__player_collide,
		sprite.w, sprite.h, T_PLAYER}));
	return (id);
}

static void	import_sprites(void *mlx, t_sprite t[NB_TEXTURES])
{
	uint8_t			i;
	static char		*paths[NB_TEXTURES] = {
		"resources/hero/idle_f2.png",
	};

	i = -1;
	while (++i < NB_TEXTURES)
		t[i].texture = mlx_png_file_to_image(mlx, paths[i], &t[i].w, &t[i].h);
}

int	__game_init(t_app *app, t_scene *scene)
{
	t_game	*game;

	game = (t_game *)scene->env;
	if (!game)
		return (APP_ERROR);
	game->camera = (t_aabb){0, 0, app->params.w, app->params.h};
	game->ecs = ecs_create(NB_COMPS, sizeof(t_vector), sizeof(t_vector),
		sizeof(t_collider), sizeof(t_sprite), sizeof(uint32_t),
		sizeof(uint32_t), sizeof(uint32_t));
	if (!game->ecs || !grid_create(&game->grid, game->camera))
		return (APP_ERROR);
	import_sprites(app->mlx, game->sprites);
	game->player = player_create(game->ecs, *game->sprites, 300 ,0);
	game->gravity = (t_vector){0, -0.98};
	return (0);
}
