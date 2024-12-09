/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:20:21 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/09 16:50:50 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static float	lerp(float v0, float v1, float t)
{
	return (v0 + t * (v1 - v0));
}

static void	collide_system(t_ecs *ecs, t_col_grid *grid)
{
	t_ecs_ulist	query;
	t_vector	*pos;
	t_collider	*col;

	query = *ecs_query(ecs, (1ULL << C_POSITION) | (1ULL << C_COLLIDER));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], C_POSITION);
		col = ecs_entity_get(ecs, query.values[query.len], C_COLLIDER);
		grid_insert(grid, col->on_collide,
			(t_aabb){pos->x, pos->y, col->w, col->h},
			query.values[query.len]);
	}
}

static void	draw_system(t_ecs *ecs, void *mlx, void *win, t_aabb cam)
{
	t_ecs_ulist	query;
	t_sprite	*img;
	t_vector	*pos;

	query = *ecs_query(ecs, (1ULL << C_SPRITE) | (1ULL << C_POSITION));
	while (query.len--)
	{
		img = ecs_entity_get(ecs, query.values[query.len], C_SPRITE);
		pos = ecs_entity_get(ecs, query.values[query.len], C_POSITION);
		if (intersects((t_aabb){pos->x, pos->y, img->w, img->h}, cam))
			mlx_put_image_to_window(mlx, win, img->texture,
				pos->x - cam.x, pos->y - cam.y);
	}
}

void	__player_collide(uint32_t player, uint32_t other, void *ptr)
{
	(void)player;
	(void)other;
	(void)ptr;
}

int	__game_update(t_app *app, t_scene *scene)
{
	t_game		*game;
	t_vector	*pos;

	game = (t_game *)scene->env;
	mlx_clear_window(app->mlx, app->win);
	collide_system(game->ecs, &game->grid);
	grid_process(&game->grid, app);
	pos = ecs_entity_get(game->ecs, game->player, C_POSITION);
	game->camera.x = lerp(game->camera.x,
			pos->x - (float)game->camera.w / 2, .05);
	game->camera.y = lerp(game->camera.y,
			pos->y - (float)game->camera.h / 2, .05);
	game->grid.bounds.x = game->camera.x;
	game->grid.bounds.y = game->camera.y;
	draw_system(game->ecs, app->mlx, app->win, game->camera);
	return (0);
}
