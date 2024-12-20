/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:18:07 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/20 19:00:15 by mrouves          ###   ########.fr       */
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

void	lifetime_system(t_ecs *ecs, t_ecs_queue *queue)
{
	t_ecs_ulist	query;
	uint32_t	*life;
	uint16_t	time;
	uint16_t	count;

	query = *ecs_query(ecs, (1ULL << COMP_LIFETIME));
	while (query.len--)
	{
		life = ecs_entity_get(ecs, query.values[query.len], COMP_LIFETIME);
		time = *life & UINT16_MAX;
		count = *life >> 16;
		if (count++ >= time)
			ecs_queue_add(queue, (t_ecs_queue_entry){
				0, query.values[query.len], 0, KILL});
		else
			*life = (count << 16) | time;
	}
}

void	game_physics(t_scene *scene, t_game *game)
{
	move_system(game->ecs);
	collide_system(game->ecs, &game->grid);
	grid_process(&game->grid, scene);
	lifetime_system(game->ecs, &game->queue);
	ecs_queue_process(game->ecs, &game->queue);
}
