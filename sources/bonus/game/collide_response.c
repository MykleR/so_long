/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collide_response.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:19:13 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/21 16:20:19 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

void	__bullet_collide(uint32_t self, uint32_t other, void *data)
{
	t_app		*app;
	t_game		*game;
	t_collider	*other_col;
	t_collider	*self_col;
	//uint32_t	*health;

	app = (t_app *)data;
	game = (t_game *)(app->scenes + app->scene_index)->env;
	other_col = ecs_entity_get(game->ecs, other, COMP_COL);
	self_col = ecs_entity_get(game->ecs, self, COMP_COL);
	//health = ecs_entity_get(game->ecs, other, COMP_HP);
	if (other_col->tag == TAG_BLOCK  || (self_col->tag == TAG_EBULLET
			&& other_col->tag == TAG_PLAYER) || (self_col->tag == TAG_PBULLET
			&& other_col->tag == TAG_ENEMY) || (other_col->tag == TAG_PBULLET
			&& self_col->tag == TAG_EBULLET) || (other_col->tag == TAG_EBULLET && self_col->tag == TAG_PBULLET))
	{
		instantiate_particule(game->ecs, (t_animation){
			((t_prog_args *)app->params.args)->imgs_other + 12, 3, 0, 3, 0},
			*(t_vector *)ecs_entity_get(game->ecs, self, COMP_POS), 9);
		ecs_entity_kill(game->ecs, self);
	}
}

void	__item_collide(uint32_t	self, uint32_t other, void *data)
{
	t_app		*app;
	t_game		*game;
	t_collider	*other_col;

	app = (t_app *)data;
	game = (t_game *)(app->scenes + app->scene_index)->env;
	other_col = ecs_entity_get(game->ecs, other, COMP_COL);
	if (other_col->tag != TAG_PLAYER)
		return ;
	ecs_entity_kill(game->ecs, self);
	game->collected++;
}
