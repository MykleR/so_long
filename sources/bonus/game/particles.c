/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   particles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:36:58 by mykle             #+#    #+#             */
/*   Updated: 2024/12/22 00:18:08 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static bool	remove_hp(t_ecs *ecs, uint32_t who, uint32_t dps)
{
	int32_t	*health;

	if (!ecs_entity_has(ecs, who, COMP_HP))
		return (true);
	health = ecs_entity_get(ecs, who, COMP_HP);
	*health -= dps;
	if (*health <= 0)
		ecs_entity_kill(ecs, who);
	return (*health > 0);
}

void	__bullet_collide(uint32_t self, uint32_t other, void *data)
{
	t_ecs		*ecs;
	t_sprite	*fx;
	t_collider	*ocol;
	t_collider	*scol;
	t_vector	*opos;

	ecs = ((t_game *)app_scene(data)->env)->ecs;
	fx = ((t_prog_args *)((t_app *)data)->params.args)->imgs_fx;
	opos = ecs_entity_get(ecs, other, COMP_POS);
	ocol = ecs_entity_get(ecs, other, COMP_COL);
	scol = ecs_entity_get(ecs, self, COMP_COL);
	if (ocol->tag == TAG_BLOCK || (scol->tag == TAG_EBULLET
			&& ocol->tag == TAG_PLAYER) || (scol->tag == TAG_PBULLET
			&& ocol->tag == TAG_ENEMY) || (ocol->tag == TAG_PBULLET
			&& scol->tag == TAG_EBULLET) || (ocol->tag == TAG_EBULLET
			&& scol->tag == TAG_PBULLET))
	{
		instantiate_particule(ecs, (t_animation){fx + 12, 3, 0, 3, 0},
			*(t_vector *)ecs_entity_get(ecs, self, COMP_POS), 9);
		ecs_entity_kill(ecs, self);
		if (!remove_hp(ecs, other, 1) && ocol->tag == TAG_ENEMY)
			instantiate_particule(ecs, (t_animation){fx + 14, 4, 0, 4, 0},
				(t_vector){opos->x - 64, opos->y - 64}, 16);
	}
}

void	__item_collide(uint32_t	self, uint32_t other, void *data)
{
	t_game		*game;
	t_collider	*other_col;
	t_collider	*self_col;

	game = app_scene(data)->env;
	other_col = ecs_entity_get(game->ecs, other, COMP_COL);
	self_col = ecs_entity_get(game->ecs, self, COMP_COL);
	if (other_col->tag != TAG_PLAYER)
		return ;
	game->collected += self_col->tag == TAG_ITEM;
	if (self_col->tag == TAG_ITEM)
		ecs_entity_kill(game->ecs, self);
	if (self_col->tag == TAG_EXIT && game->collected >= game->to_collect)
		app_load(data, 0);
}

uint32_t	instantiate_particule(t_ecs *ecs, t_animation anim,
				t_vector pos, int lifetime)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, COMP_POS, &pos);
	ecs_entity_add(ecs, id, COMP_IMG, anim.frames);
	ecs_entity_add(ecs, id, COMP_ANIM, &anim);
	if (lifetime)
		ecs_entity_add(ecs, id, COMP_LIFETIME, &lifetime);
	return (id);
}

uint32_t	instantiate_tile(t_ecs *ecs, t_sprite *imgs,
				t_vector pos, t_tile tile)
{
	uint32_t	id;
	t_collider	collider;

	if (tile == FLOOR || tile == SPAWN)
		return (UINT32_MAX);
	imgs += tile == ITEM;
	if (tile == EXIT)
		imgs += 5;
	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, COMP_IMG, imgs);
	ecs_entity_add(ecs, id, COMP_POS, &pos);
	collider = (t_collider){__item_collide, imgs->w, imgs->h, TAG_ITEM};
	if (tile == WALL)
		collider = (t_collider){NULL, imgs->w, imgs->h, TAG_BLOCK};
	if (tile == EXIT)
		collider.tag = TAG_EXIT;
	if (tile == EXIT || tile == WALL || tile == ITEM)
		ecs_entity_add(ecs, id, COMP_COL, &collider);
	if (tile == ITEM || tile == EXIT)
		ecs_entity_add(ecs, id, COMP_ANIM, &((t_animation){imgs, 8, 0, 4, 0}));
	return (id);
}
