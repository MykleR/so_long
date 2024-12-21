/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 12:52:16 by mykle             #+#    #+#             */
/*   Updated: 2024/12/21 17:40:51 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

uint32_t	instantiate_player(t_ecs *ecs, t_sprite sprite,
				float x, float y)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, COMP_POS, &(t_vector){x, y});
	ecs_entity_add(ecs, id, COMP_IMG, &sprite);
	ecs_entity_add(ecs, id, COMP_VEL, &(t_vector){0, 0});
	ecs_entity_add(ecs, id, COMP_COL, &(t_collider){
			__player_collide, sprite.w, sprite.h, TAG_PLAYER});
	ecs_entity_add(ecs, id, COMP_GRAV, &(t_vector){
			S_WORLD_GRAVX, S_WORLD_GRAVY});
	ecs_entity_add(ecs, id, COMP_HP, &(int32_t){S_PLAYER_HP});
	return (id);
}

uint32_t	instantiate_pbullet(t_ecs *ecs, t_sprite *imgs,
				t_vector pos, t_vector vel)
{
	uint32_t	id;

	id = instantiate_particule(ecs, (t_animation){imgs, 2, 0, 4, 0},
			pos, S_BULLET_DURATION);
	ecs_entity_add(ecs, id, COMP_VEL, &vel);
	ecs_entity_add(ecs, id, COMP_COL, &((t_collider){
			__bullet_collide, imgs->w, imgs->h, TAG_PBULLET}));
	return (id);
}

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

void	__player_collide(uint32_t player, uint32_t other, void *data)
{
	t_app		*app;
	t_game		*game;
	t_collider	*ocol;

	app = (t_app *)data;
	game = (t_game *)(app->scenes + app->scene_index)->env;
	ocol = ecs_entity_get(game->ecs, other, COMP_COL);
	if (ocol->tag == TAG_BLOCK)
		resolve_player_to_block((t_box_resolve){
			ecs_entity_get(game->ecs, player, COMP_COL), ocol,
			ecs_entity_get(game->ecs, player, COMP_POS),
			ecs_entity_get(game->ecs, other, COMP_POS)},
			ecs_entity_get(game->ecs, player, COMP_VEL));
}
