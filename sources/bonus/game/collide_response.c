/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collide_response.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:19:13 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/20 18:50:06 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static void	resolve_player_to_block(t_box_resolve info, t_vector *pvel)
{
	info.p1->y -= pvel->y;
	if (intersects((t_aabb){info.p1->x, info.p1->y, info.c1->w, info.c1->h},
		(t_aabb){info.p2->x, info.p2->y, info.c2->w, info.c2->h}))
	{
		if (pvel->x > 0)
			info.p1->x = info.p2->x - info.c1->w;
		if (pvel->x < 0)
			info.p1->x = info.p2->x + info.c2->w;
		pvel->x = (pvel->x > 0) - (pvel->x < 0);
		pvel->y *= S_PLAYER_FRICTION;
	}
	info.p1->x -= pvel->x;
	info.p1->y += pvel->y;
	if (intersects((t_aabb){info.p1->x, info.p1->y, info.c1->w, info.c1->h},
		(t_aabb){info.p2->x, info.p2->y, info.c2->w, info.c2->h}))
	{
		if (pvel->y > 0)
			info.p1->y = info.p2->y - info.c1->h;
		if (pvel->y < 0)
			info.p1->y = info.p2->y + info.c2->h;
		pvel->y = 0;
		pvel->x *= S_PLAYER_FRICTION;
	}
	info.p1->x += pvel->x;
}

void	__player_collide(uint32_t player, uint32_t other, void *ptr)
{
	t_game		*game;
	t_collider	*ocol;

	game = (t_game *)((t_scene *)ptr)->env;
	ocol = ecs_entity_get(game->ecs, other, COMP_COL);
	if (ocol->tag != TAG_BLOCK)
		return ;
	resolve_player_to_block((t_box_resolve){
		ecs_entity_get(game->ecs, player, COMP_COL), ocol,
		ecs_entity_get(game->ecs, player, COMP_POS),
		ecs_entity_get(game->ecs, other, COMP_POS)},
		ecs_entity_get(game->ecs, player, COMP_VEL));
}

void	__bullet_collide(uint32_t self, uint32_t other, void *data)
{
	t_game		*game;
	t_collider	*other_col;

	game = (t_game *)((t_scene *)data)->env;
	other_col = ecs_entity_get(game->ecs, other, COMP_COL);
	if (other_col->tag == TAG_BLOCK && ecs_entity_alive(game->ecs, other))
		ecs_entity_kill(game->ecs, self);
}
