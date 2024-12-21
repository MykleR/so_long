/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   particles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:36:58 by mykle             #+#    #+#             */
/*   Updated: 2024/12/21 19:37:29 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static void	remove_hp(t_ecs *ecs, uint32_t who, uint32_t dps)
{
	int32_t	*health;

	if (!ecs_entity_has(ecs, who, COMP_HP))
		return ;
	health = ecs_entity_get(ecs, who, COMP_HP);
	*health -= dps;
	if (*health <= 0)
		ecs_entity_kill(ecs, who);
}

void	__bullet_collide(uint32_t self, uint32_t other, void *data)
{
	t_app		*app;
	t_game		*game;
	t_collider	*other_col;
	t_collider	*self_col;

	app = (t_app *)data;
	game = (t_game *)(app->scenes + app->scene_index)->env;
	other_col = ecs_entity_get(game->ecs, other, COMP_COL);
	self_col = ecs_entity_get(game->ecs, self, COMP_COL);
	if (other_col->tag == TAG_BLOCK || (self_col->tag == TAG_EBULLET
			&& other_col->tag == TAG_PLAYER) || (self_col->tag == TAG_PBULLET
			&& other_col->tag == TAG_ENEMY) || (other_col->tag == TAG_PBULLET
			&& self_col->tag == TAG_EBULLET) || (other_col->tag == TAG_EBULLET
			&& self_col->tag == TAG_PBULLET))
	{
		instantiate_particule(game->ecs, (t_animation){
			((t_prog_args *)app->params.args)->imgs_fx + 12, 3, 0, 3, 0},
			*(t_vector *)ecs_entity_get(game->ecs, self, COMP_POS), 9);
		ecs_entity_kill(game->ecs, self);
		remove_hp(game->ecs, other, 1);
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

	if (tile == FLOOR || tile == SPAWN)
		return (UINT32_MAX);
	imgs += tile == ITEM;
	if (tile == EXIT)
		imgs += 5;
	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, COMP_IMG, imgs);
	ecs_entity_add(ecs, id, COMP_POS, &pos);
	if (tile == WALL)
		ecs_entity_add(ecs, id, COMP_COL, &((t_collider){
				NULL, imgs->w, imgs->h, TAG_BLOCK}));
	if (tile == ITEM)
		ecs_entity_add(ecs, id, COMP_COL, &((t_collider){
				__item_collide, imgs->w, imgs->h, TAG_ITEM}));
	if (tile == ITEM || tile == EXIT)
		ecs_entity_add(ecs, id, COMP_ANIM, &((t_animation){imgs, 8, 0, 4, 0}));
	return (id);
}
