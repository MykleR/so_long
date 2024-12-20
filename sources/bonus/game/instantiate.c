/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instantiate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 20:27:46 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/20 18:15:02 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

uint32_t	instantiate_player(t_ecs *ecs, t_sprite sprite, float x, float y)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, COMP_POS, &((t_vector){x, y}));
	ecs_entity_add(ecs, id, COMP_IMG, &sprite);
	ecs_entity_add(ecs, id, COMP_VEL, &((t_vector){0, 0}));
	ecs_entity_add(ecs, id, COMP_COL, &((t_collider){
			__player_collide, sprite.w, sprite.h, TAG_PLAYER}));
	ecs_entity_add(ecs, id, COMP_GRAV, &((t_vector){
			S_WORLD_GRAVX, S_WORLD_GRAVY}));
	return (id);
}

uint32_t	instantiate_tile(t_ecs *ecs, t_sprite *imgs,
				t_vector pos, t_tile tile)
{
	uint32_t	id;

	if (tile != WALL)
		return (UINT32_MAX);
	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, COMP_IMG, imgs);
	ecs_entity_add(ecs, id, COMP_POS, &pos);
	ecs_entity_add(ecs, id, COMP_COL, &((t_collider){
			NULL, imgs->w, imgs->h, TAG_BLOCK}));
	return (id);
}

uint32_t	instantiate_bullet(t_ecs *ecs, t_sprite *imgs,
				t_vector pos, t_vector vel)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, COMP_POS, &pos);
	ecs_entity_add(ecs, id, COMP_VEL, &vel);
	ecs_entity_add(ecs, id, COMP_IMG, imgs);
	ecs_entity_add(ecs, id, COMP_COL, &((t_collider){
			__bullet_collide, imgs->w, imgs->h, TAG_BULLET}));
	ecs_entity_add(ecs, id, COMP_ANIM, &((t_animation){
			imgs, 2, 0, 4, 0, 1, 1}));
	return (id);
}

uint32_t	instantiate_particule(t_ecs *ecs, t_sprite *imgs, t_vector pos)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, COMP_POS, &pos);
	ecs_entity_add(ecs, id, COMP_IMG, imgs);
	ecs_entity_add(ecs, id, COMP_ANIM, &((t_animation){
			imgs, 5, 0, 4, 0, 1, 1}));
	return (id);
}
