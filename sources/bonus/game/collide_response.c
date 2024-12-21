/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collide_response.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:19:13 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/21 12:53:10 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

void	__bullet_collide(uint32_t self, uint32_t other, void *data)
{
	t_game		*game;
	t_collider	*other_col;

	game = (t_game *)((t_scene *)data)->env;
	other_col = ecs_entity_get(game->ecs, other, COMP_COL);
	if (other_col->tag == TAG_BLOCK && ecs_entity_alive(game->ecs, other))
		ecs_entity_kill(game->ecs, self);
}

void	__item_collide(uint32_t	self, uint32_t other, void *data)
{
	t_game		*game;
	t_collider	*other_col;

	game = (t_game *)((t_scene *)data)->env;
	other_col = ecs_entity_get(game->ecs, other, COMP_COL);
	if (other_col->tag != TAG_PLAYER)
		return ;
	ecs_entity_kill(game->ecs, self);
	game->collected++;
}

void	__enemy_collide(uint32_t self, uint32_t other, void *data)
{
	(void)self;
	(void)other;
	(void)data;
}
