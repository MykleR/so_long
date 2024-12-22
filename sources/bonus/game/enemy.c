/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 13:05:47 by mykle             #+#    #+#             */
/*   Updated: 2024/12/21 23:51:37 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

uint32_t	instantiate_ebullet(t_ecs *ecs, t_sprite *imgs,
				t_vector pos, t_vector vel)
{
	uint32_t	id;

	id = instantiate_particule(ecs, (t_animation){imgs, 2, 0, 4, 0}, pos, 0);
	ecs_entity_add(ecs, id, COMP_VEL, &vel);
	ecs_entity_add(ecs, id, COMP_COL, &((t_collider){
			__bullet_collide, imgs->w, imgs->h, TAG_EBULLET}));
	return (id);
}

uint32_t	instantiate_enemy(t_ecs *ecs, t_sprite *imgs, t_vector pos)
{
	uint32_t	id;

	id = instantiate_particule(ecs, (t_animation){imgs, 5, 0, 4, 0}, pos, 0);
	ecs_entity_add(ecs, id, COMP_VEL, &((t_vector){0, 0}));
	ecs_entity_add(ecs, id, COMP_COL, &((t_collider){
			__enemy_collide, imgs->w, imgs->h, TAG_ENEMY}));
	ecs_entity_add(ecs, id, COMP_ENEMY, &(uint32_t){S_ENEMY_SHOOTRATE});
	ecs_entity_add(ecs, id, COMP_HP, &(int32_t){S_ENEMY_HP});
	return (id);
}

void	__enemy_collide(uint32_t self, uint32_t other, void *data)
{
	t_game		*game;
	t_collider	*other_col;
	t_vector	*vel;

	game = app_scene(data)->env;
	other_col = ecs_entity_get(game->ecs, other, COMP_COL);
	vel = ecs_entity_get(game->ecs, self, COMP_VEL);
	if (other_col->tag == TAG_PLAYER)
		*vel = (t_vector){0, 0};
}
