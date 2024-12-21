/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 13:05:47 by mykle             #+#    #+#             */
/*   Updated: 2024/12/21 13:06:14 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

uint32_t	instantiate_ebullet(t_ecs *ecs, t_sprite *imgs, t_vector pos, t_vector vel)
{
	uint32_t	id;

	id = instantiate_particule(ecs, (t_animation){
			imgs, 2, 0, 4, 0, 1, 1}, pos, 0);
	ecs_entity_add(ecs, id, COMP_VEL, &vel);
	ecs_entity_add(ecs, id, COMP_COL, &((t_collider){
			__bullet_collide, imgs->w, imgs->h, TAG_EBULLET}));
	return (id);
}

uint32_t	instantiate_enemy(t_ecs *ecs, t_sprite *imgs,
				t_vector pos, uint32_t player_id)
{
	uint32_t	id;

	id = instantiate_particule(ecs, (t_animation){
		imgs, 5, 0, 4, 0, 1, 1}, pos, 0);
	ecs_entity_add(ecs, id, COMP_VEL, &((t_vector){0, 0}));
	ecs_entity_add(ecs, id, COMP_COL, &((t_collider){
			__enemy_collide, imgs->w, imgs->h, TAG_ENEMY}));
	ecs_entity_add(ecs, id, COMP_ENEMY, &player_id);
	return (id);
}
