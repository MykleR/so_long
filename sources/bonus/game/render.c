/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:17:50 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/20 19:35:40 by mrouves          ###   ########.fr       */
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
		anim->count = ((anim->count + 1) * anim->play) % anim->delay;
		if (anim->play && !anim->count)
			anim->frame = (anim->frame + 1) % anim->nb_frames;
		if (!anim->play)
			anim->frame = 0;
		*img = anim->frames[anim->frame];
	}
}

static void	move_camera_target(t_ecs *ecs, t_aabb *camera, uint32_t target)
{
	t_vector	*pos;

	pos = ecs_entity_get(ecs, target, COMP_POS);
	camera->x = lerp(camera->x, pos->x - (float)camera->w / 2, 0.08);
	camera->y = lerp(camera->y, pos->y - (float)camera->h / 2, 0.08);
}

void	game_render(t_app *app, t_game *game)
{
	t_sprite	*bg;

	bg = ((t_prog_args *)app->params.args)->imgs_env;
	mlx_clear_window(app->mlx, app->win);
	mlx_put_image_to_window(app->mlx, app->win, bg->texture,
		(app->params.w >> 1) - (bg->w >> 1),
		(app->params.h >> 1) - (bg->h >> 1));
	move_camera_target(game->ecs, &game->camera, game->player);
	animation_system(game->ecs);
	draw_system(game->ecs, app->mlx, app->win, game->camera);
}
