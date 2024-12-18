/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:20:21 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/18 16:36:21 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static float	lerp(float v0, float v1, float t)
{
	return (v0 + t * (v1 - v0));
}

static void move_system(t_ecs *ecs, t_vector grav)
{
	t_ecs_ulist query;
	t_vector	*pos;
	t_vector	*vel;

	query = *ecs_query(ecs, (1ULL << C_POSITION) | (1ULL << C_VELOCITY));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], C_POSITION);
		vel = ecs_entity_get(ecs, query.values[query.len], C_VELOCITY);
		if (ecs_entity_has(ecs, query.values[query.len], C_GRAVITY))
		{
			vel->x += grav.x;
			vel->y += grav.y;
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

	query = *ecs_query(ecs, (1ULL << C_POSITION) | (1ULL << C_COLLIDER));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], C_POSITION);
		col = ecs_entity_get(ecs, query.values[query.len], C_COLLIDER);
		box = (t_aabb){pos->x, pos->y, col->w, col->h};
		grid_insert(grid, col->on_collide, box, query.values[query.len]);
	}
}

static void	draw_system(t_ecs *ecs, void *mlx, void *win, t_aabb cam)
{
	t_ecs_ulist	query;
	t_sprite	*img;
	t_vector	*pos;
	t_aabb		box;

	query = *ecs_query(ecs, (1ULL << C_SPRITE) | (1ULL << C_POSITION));
	while (query.len--)
	{
		img = ecs_entity_get(ecs, query.values[query.len], C_SPRITE);
		pos = ecs_entity_get(ecs, query.values[query.len], C_POSITION);
		box = (t_aabb){pos->x, pos->y, img->w, img->h};
		if (intersects(box, cam))
			mlx_put_image_to_window(mlx, win, img->texture,
				pos->x - cam.x, pos->y - cam.y);
	}
}

static void	resolve_player_to_block(t_box_resolve info, t_vector *pvel)
{
	info.p1->y -= pvel->y;
	if (intersects((t_aabb){info.p1->x, info.p1->y, info.c1->w, info.c1->h},
			(t_aabb){info.p2->x, info.p2->y, info.c2->w, info.c2->h}))
	{
		if (pvel->x > 0)
			info.p1->x = info.p2->x - info.c1->w;
		if (pvel->x < 0)
			info.p1->x = info.p2->x + info.c2->w;
		pvel->x = 0;
	}
	info.p1->x -= pvel->x;
	info.p1->y += pvel->y;
	if (intersects((t_aabb){info.p1->x, info.p1->y, info.c1->w, info.c1->h},
			(t_aabb){info.p2->x, info.p2->y, info.c2->w, info.c2->h}))
	{
		if (pvel->y > 0)
			info.p1->y = info.p2->y - info.c1->h;
		if (pvel->y < 0)
			info.p1->y = info.p2->y + info.c2->h;
		pvel->y = 0;
		pvel->x *= S_PLAYER_FRICTION;
	}
	info.p1->x += pvel->x;
}

void	__player_collide(uint32_t player, uint32_t other, void *ptr)
{
	t_game		*game;
	t_collider	*ocol;

	game = (t_game *)((t_scene *)ptr)->env;
	ocol = ecs_entity_get(game->ecs, other, C_COLLIDER);
	if (ocol->tag != T_BLOCK)
		return ;
	resolve_player_to_block((t_box_resolve){
		ecs_entity_get(game->ecs, player, C_COLLIDER), ocol,
		ecs_entity_get(game->ecs, player, C_POSITION),
		ecs_entity_get(game->ecs, other, C_POSITION)},
		ecs_entity_get(game->ecs, player, C_VELOCITY));
}

int	__game_update(t_app *app, t_scene *scene)
{
	t_game		*game;
	t_vector	*pos;
	t_sprite	*bg;

	game = (t_game *)scene->env;
	mlx_clear_window(app->mlx, app->win);
	move_system(game->ecs, game->gravity);
	collide_system(game->ecs, &game->grid);
	grid_process(&game->grid, scene);
	pos = ecs_entity_get(game->ecs, game->player, C_POSITION);
	game->camera.x = lerp(game->camera.x, pos->x - (float)game->camera.w / 2, 0.08);
	game->camera.y = lerp(game->camera.y, pos->y - (float)game->camera.h / 2, 0.08);
	game->grid.bounds.x = game->camera.x;
	game->grid.bounds.y = game->camera.y;
	bg = ((t_prog_args *)app->params.args)->imgs_bg;
	mlx_put_image_to_window(app->mlx, app->win, bg->texture,
			(app->params.w >> 1) - (bg->w >> 1),
			(app->params.h >> 1) - (bg->h >> 1));
	draw_system(game->ecs, app->mlx, app->win, game->camera);
	return (0);
}
