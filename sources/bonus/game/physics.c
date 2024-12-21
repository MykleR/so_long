/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:18:07 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/21 16:34:17 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static void	move_system(t_ecs *ecs)
{
	t_ecs_ulist	query;
	uint32_t	id;
	t_vector	*pos;
	t_vector	*vel;
	t_vector	*grav;

	query = *ecs_query(ecs, (1ULL << COMP_POS) | (1ULL << COMP_VEL));
	while (query.len--)
	{
		id = query.values[query.len];
		pos = ecs_entity_get(ecs, id, COMP_POS);
		vel = ecs_entity_get(ecs, id, COMP_VEL);
		if (ecs_entity_has(ecs, id, COMP_GRAV))
		{
			grav = ecs_entity_get(ecs, id, COMP_GRAV);
			vel->x += grav->x;
			vel->y += grav->y;
		}
		pos->x += vel->x;
		pos->y += vel->y;
	}
}

static void	collide_system(t_ecs *ecs, t_col_grid *grid)
{
	t_ecs_ulist	query;
	t_vector	*pos;
	t_collider	*col;
	t_aabb		box;

	query = *ecs_query(ecs, (1ULL << COMP_POS) | (1ULL << COMP_COL));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], COMP_POS);
		col = ecs_entity_get(ecs, query.values[query.len], COMP_COL);
		box = (t_aabb){pos->x, pos->y, col->w, col->h};
		grid_insert(grid, col->on_collide, box, query.values[query.len]);
	}
}

static void	lifetime_system(t_ecs *ecs, t_ecs_queue *queue)
{
	t_ecs_ulist	query;
	uint32_t	*time;

	query = *ecs_query(ecs, (1ULL << COMP_LIFETIME));
	while (query.len--)
	{
		time = ecs_entity_get(ecs, query.values[query.len], COMP_LIFETIME);
		*time += (1 << 16);
		if ((*time >> 16) >= (*time & UINT16_MAX))
			ecs_queue_add(queue, (t_ecs_queue_entry){
				0, query.values[query.len], 0, KILL});
	}
}

static void	enemy_system(t_ecs *ecs, uint32_t player, t_sprite *bullet_imgs)
{
	t_ecs_ulist	query;
	t_vector	*target_pos;
	t_vector	*self_pos;
	uint32_t	*shoot_rate;
	float		angle;

	query = *ecs_query(ecs, (1ULL << COMP_ENEMY) | (1ULL << COMP_VEL));
	while (query.len--)
	{
		target_pos = ecs_entity_get(ecs, player, COMP_POS);
		self_pos = ecs_entity_get(ecs, query.values[query.len], COMP_POS);
		shoot_rate = ecs_entity_get(ecs, query.values[query.len], COMP_ENEMY);
		angle = atan2((target_pos->y - self_pos->y), (target_pos->x - self_pos->x));
		*(t_vector *)ecs_entity_get(ecs, query.values[query.len], COMP_VEL) =
			(t_vector){cos(angle) * S_ENEMY_SPEED, sin(angle) * S_ENEMY_SPEED};
		*shoot_rate += (1 << 16);
		if ((*shoot_rate >> 16) >= (*shoot_rate & UINT16_MAX))
		{
			*shoot_rate &= UINT16_MAX;
			instantiate_ebullet(ecs, bullet_imgs, *self_pos, (t_vector){
				cos(angle) * S_ENEMY_BULLET_F, sin(angle) * S_ENEMY_BULLET_F});
		}
	}
}

void	game_physics(t_app *app, t_game *game)
{
	move_system(game->ecs);
	enemy_system(game->ecs, game->player,
			((t_prog_args *)app->params.args)->imgs_fx + 8);
	collide_system(game->ecs, &game->grid);
	grid_process(&game->grid, app);
	lifetime_system(game->ecs, &game->queue);
	ecs_queue_process(game->ecs, &game->queue);
}
