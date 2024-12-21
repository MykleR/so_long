/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systems_physics.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:18:07 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/21 20:01:10 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static inline void	vec_add(t_vector a, t_vector b, t_vector *out)
{
	*out = (t_vector){a.x + b.x, a.y + b.y};
}

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
		grav = ecs_entity_get(ecs, id, COMP_GRAV);
		if (ecs_entity_has(ecs, id, COMP_GRAV))
			vec_add(*vel, *grav, vel);
		vec_add(*pos, *vel, pos);
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

static void	enemy_system(t_ecs *ecs, uint32_t player, t_sprite *bullet_imgs)
{
	t_ecs_ulist	query;
	t_vector	*target;
	t_vector	*pos;
	uint32_t	*rate;
	float		angle;

	target = ecs_entity_get(ecs, player, COMP_POS);
	query = *ecs_query(ecs, (1ULL << COMP_ENEMY) | (1ULL << COMP_VEL));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], COMP_POS);
		rate = ecs_entity_get(ecs, query.values[query.len], COMP_ENEMY);
		angle = atan2((target->y - pos->y), (target->x - pos->x));
		*(t_vector *)ecs_entity_get(ecs, query.values[query.len], COMP_VEL) =
			(t_vector){cos(angle) * S_ENEMY_SPEED, sin(angle) * S_ENEMY_SPEED};
		*rate += (1 << 16);
		if ((*rate >> 16) >= (*rate & UINT16_MAX))
		{
			*rate &= UINT16_MAX;
			instantiate_ebullet(ecs, bullet_imgs, *pos, (t_vector){
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
	if (*(int32_t *)ecs_entity_get(game->ecs, game->player, COMP_HP) <= 0)
		app_load(app, 2);
}
