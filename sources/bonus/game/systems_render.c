/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systems_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:17:50 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/22 00:56:22 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static inline float	lerp(float v0, float v1, float t)
{
	return (v0 + t * (v1 - v0));
}

static void	draw_system(t_ecs *ecs, void *mlx, void *win, t_aabb cam)
{
	t_ecs_ulist	query;
	t_sprite	*img;
	t_vector	*pos;
	t_aabb		box;

	query = *ecs_query(ecs, (1ULL << COMP_IMG) | (1ULL << COMP_POS));
	while (query.len--)
	{
		img = ecs_entity_get(ecs, query.values[query.len], COMP_IMG);
		pos = ecs_entity_get(ecs, query.values[query.len], COMP_POS);
		box = (t_aabb){pos->x, pos->y, img->w, img->h};
		if (intersects(box, cam))
			mlx_put_image_to_window(mlx, win, img->texture,
				pos->x - cam.x, pos->y - cam.y);
	}
}

void	animation_system(t_ecs *ecs)
{
	t_ecs_ulist	query;
	t_animation	*anim;
	t_sprite	*img;

	query = *ecs_query(ecs, (1ULL << COMP_IMG | (1ULL << COMP_ANIM)));
	while (query.len--)
	{
		anim = ecs_entity_get(ecs, query.values[query.len], COMP_ANIM);
		img = ecs_entity_get(ecs, query.values[query.len], COMP_IMG);
		if (!anim->delay || !anim->nb_frames)
			anim->frame = 0;
		else
		{
			anim->count = (anim->count + 1) % anim->delay;
			if (!anim->count)
				anim->frame = (anim->frame + 1) % anim->nb_frames;
		}
		*img = anim->frames[anim->frame];
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
			ecs_queue_add(queue, (t_ecs_queue_entry){0,
				query.values[query.len], 0, KILL});
	}
}

void	game_render(t_app *app, t_game *game)
{
	int32_t		hp;
	t_sprite	*hud;
	t_sprite	*bg;
	t_vector	*pos;
	t_aabb		*camera;

	camera = &game->camera;
	bg = ((t_prog_args *)app->params.args)->imgs_env;
	hp = *(int32_t *)ecs_entity_get(game->ecs, game->player, COMP_HP);
	hud = ((t_prog_args *)app->params.args)->imgs_hud;
	hud += (int)roundf(hp / ((float)S_PLAYER_HP) * (N_IMGS_HUD - 1));
	lifetime_system(game->ecs, &game->queue);
	ecs_queue_process(game->ecs, &game->queue);
	mlx_clear_window(app->mlx, app->win);
	mlx_put_image_to_window(app->mlx, app->win, bg->texture,
		(app->params.w >> 1) - (bg->w >> 1),
		(app->params.h >> 1) - (bg->h >> 1));
	pos = ecs_entity_get(game->ecs, game->player, COMP_POS);
	camera->x = lerp(camera->x, pos->x - (float)(camera->w >> 1), 0.08);
	camera->y = lerp(camera->y, pos->y - (float)(camera->h >> 1), 0.08);
	animation_system(game->ecs);
	draw_system(game->ecs, app->mlx, app->win, *camera);
	mlx_put_image_to_window(app->mlx, app->win, hud->texture, 0, 0);
}
