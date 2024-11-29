/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualization.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:58:46 by mykle             #+#    #+#             */
/*   Updated: 2024/11/29 14:07:56 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application.h"
#include "ecs.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

#define POSITION 0
#define VELOCITY 1
#define COLLIDER 2

#define GRID_CELL_SIZE 50
#define GRID_CELL_CAP 10

typedef enum e_collision_tag
{
	STATIC,
	MOVING,
}	t_collision_tag;

typedef struct s_collider
{
	uint16_t		w;
	uint16_t		h;
	t_collision_tag	tag;
	bool			is_colliding;
}	t_collider;

typedef struct s_vector
{
	float	x;
	float	y;
}	t_vector;

typedef struct
{
    int	x;
	int	y;
    int	w;
	int	h;
}	t_aabb;

typedef struct
{
	t_ecs_ulist	**cells;
	uint16_t	cell_size;
	uint16_t	cell_row;
	t_vector	pos;
} t_collision_grid;


typedef struct	s_env
{
	t_ecs	*ecs;
	t_collision_grid *grid;
}	t_env;

static bool	grid_fill(t_collision_grid *grid)
{
	uint32_t	i;

	grid->cells = ft_calloc(sizeof(t_ecs_ulist *), grid->cell_row * grid->cell_row);
	if (__builtin_expect(!grid->cells, 0))
		return (false);
	i = -1;
	while (++i < grid->cell_row * grid->cell_row)
	{
		*(grid->cells + i) = list_create(GRID_CELL_CAP);
		if (!(*(grid->cells + i)))
		{
			free(grid->cells);
			return (false);
		}
	}
	return (true);
}

t_collision_grid	*grid_create(t_vector pos, uint16_t w)
{
	t_collision_grid	*grid;

	grid = malloc(sizeof(t_collision_grid));
	if (__builtin_expect(!grid, 0))
		return (NULL);
	grid->cell_size = GRID_CELL_SIZE;
	grid->cell_row = w;
	grid->pos = pos;
	if (__builtin_expect(grid_fill(grid), 1))
		return (grid);
	free(grid);
	return (NULL);
	
}

void	grid_destroy(t_collision_grid *grid)
{
	uint32_t	i;

	i = -1;
	while (++i < grid->cell_row * grid->cell_row)
		list_destroy(*(grid->cells + i));
	free(grid->cells);
	free(grid);
}

void	grid_clear(t_collision_grid *grid)
{
	uint32_t	i;

	i = -1;
	while (++i < grid->cell_row * grid->cell_row)
	{
		ft_memset(grid->cells[i]->values, 0, sizeof(uint32_t) * grid->cells[i]->len);
		grid->cells[i]->len = 0;
	}
}

void	grid_insert(t_collision_grid *grid, uint32_t id, t_vector pos)
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	index;

	j = floor((pos.x - grid->pos.x) / grid->cell_size);
	i = floor((pos.y - grid->pos.y) / grid->cell_size);
	index = i * grid->cell_row + j;
	list_add(grid->cells[index], id);
}

static bool	intersects(t_aabb b, t_aabb a) {
    return (b.x <= a.x + a.w && b.x + b.w >= a.x &&
             b.y <= a.y + a.h & b.y + b.h >= a.y);
}

static void check_collisions(t_ecs* ecs, t_ecs_ulist l1, t_ecs_ulist l2, void (*f)(t_ecs *, uint32_t, uint32_t))
{
	t_collider	*col1;
	t_collider	*col2;
	t_vector	*pos1;
	t_vector	*pos2;
	uint32_t	i;

	while(l1.len--)
	{
		i = l2.len;
		while(i--)
		{
			col1 = ecs_entity_get(ecs, l1.values[l1.len], COLLIDER);
			col2 = ecs_entity_get(ecs, l2.values[i], COLLIDER);
			pos1 = ecs_entity_get(ecs, l1.values[l1.len], POSITION);
			pos2 = ecs_entity_get(ecs, l2.values[i], POSITION);
			if (l2.values[i] != l1.values[l1.len] && intersects(
				(t_aabb){pos1->x, pos1->y, col1->w, col1->h},
				(t_aabb){pos2->x, pos2->y, col2->w, col2->h}))
				f(ecs, l1.values[l1.len], l2.values[i]);
		}
	}
}

bool	is_ingrid(int16_t i, int16_t j, uint16_t row)
{
    return (i >= 0 && i < row && j >= 0 && j < row);
}

/*
void	grid_process(t_collision_grid *grid, t_ecs *ecs, void (*f)(t_ecs *, uint32_t, uint32_t))
{
	static int	offset_r[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    static int	offset_c[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
	uint32_t	i;
	uint32_t	j;

	i = -1;
	while (++i < grid->cell_row * grid->cell_row)
	{
		check_collisions(ecs, *(grid->cells[i]), *(grid->cells[i]), f);
		j = -1;
		while (++j < 8)
			if (is_ingrid(i / grid->cell_row + offset_r[j], i % grid->cell_row + offset_c[j], grid->cell_row))
				check_collisions(ecs, *grid->cells[i], *grid->cells[i + grid->cell_row * offset_r[j] + offset_c[j]], f);
	}
}
*/
typedef void (*collide_event) (t_ecs *, uint32_t, uint32_t);

static void	check_neightbours(t_collision_grid *grid, t_ecs *ecs,
				collide_event callback, uint32_t i)
{
	static const int	offsets[16] = {
		-1, -1, -1, 0, 0, 1, 1, 1, -1,  0,  1, -1, 1, -1, 0, 1};
	uint32_t	j;
	uint32_t	nbour;

	j = -1;
	while (++j < 8)
	{
		nbour = i + grid->cell_row * offsets[j] + offsets[j + 8];
		if (is_ingrid(nbour / grid->cell_row, nbour % grid->cell_row,
				grid->cell_row))
			check_collisions(ecs, *grid->cells[i], *grid->cells[nbour], callback);
	}
}

void	grid_process(t_collision_grid *grid, t_ecs *ecs, collide_event callback)
{
	uint32_t	i;

	i = -1;
	while (++i < grid->cell_row * grid->cell_row)
	{
		check_collisions(ecs, *(grid->cells[i]), *(grid->cells[i]), callback);
		check_neightbours(grid, ecs, callback, i);
	}
}

// Exemple de callback pour afficher les paires en collision
void collisionCallback(t_ecs *ecs, uint32_t a, uint32_t b)
{
	t_collider	*c1 = ecs_entity_get(ecs, a, COLLIDER);
	t_collider	*c2 = ecs_entity_get(ecs, b, COLLIDER);
	c1->is_colliding = true;
	c2->is_colliding = true;
}

float	fclampf(float x, float max, float min)
{
	return (fmaxf(fminf(max, x), min));
}

bool	is_in_bound(t_vector pos, t_vector bound, int w, int h)
{
	return (pos.x >= bound.x && pos.x <= bound.x + w
		&& pos.y >= bound.y && pos.y <= bound.y + h);
}

static uint32_t box_create(t_ecs *ecs, float x, float y, float vx, float vy)
{
	uint32_t	id;

	id = ecs_entity_create(ecs);
	ecs_entity_add(ecs, id, POSITION, &(t_vector){x, y});
	ecs_entity_add(ecs, id, VELOCITY, &(t_vector){vx , vy});
	ecs_entity_add(ecs, id, COLLIDER, &(t_collider){20, 20, MOVING, 0});
	return (id);
}

void	box_draw(t_app *app, t_vector pos, int w, int h, uint32_t color)
{
	int16_t	i;
	int		ww;
	int		wh;

	ww = app->params.width;
	wh = app->params.height;
	i = -1;
	while (++i < w)
	{
		if (is_in_bound((t_vector){pos.x + i, pos.y}, (t_vector){1, 1}, ww - 1, wh - 1))
			mlx_pixel_put(app->mlx, app->win, pos.x + i, pos.y, color);
		if (is_in_bound((t_vector){pos.x + i, pos.y + h}, (t_vector){1, 1}, ww - 1, wh - 1))
			mlx_pixel_put(app->mlx, app->win, pos.x + i, pos.y + h, color);
	}
	i = -1;
	while (++i < h)
	{
		if (is_in_bound((t_vector){pos.x, pos.y + i}, (t_vector){0, 0}, ww - 1, wh - 1))
			mlx_pixel_put(app->mlx, app->win, pos.x , pos.y + i, color);
		if (is_in_bound((t_vector){pos.x + w, pos.y + i}, (t_vector){0, 0}, ww - 1, wh - 1))
			mlx_pixel_put(app->mlx, app->win, pos.x + w, pos.y + i, color);
	}
}

void	grid_draw(t_app *app, t_collision_grid *grid)
{
	uint16_t	i;
	uint16_t	j;

	i = -1;
	while (++i < grid->cell_row)
	{
		j = -1;
		while (++j < grid->cell_row)
			box_draw(app, (t_vector){grid->pos.x + j * grid->cell_size,
				grid->pos.y + i * grid->cell_size},
				grid->cell_size, grid->cell_size, 0x50FFFFFF);
	}
}

void	box_system(t_ecs *ecs, t_app *app, t_collision_grid *grid)
{
	t_ecs_ulist	query;
	t_vector	*pos;
	t_vector	*vel;
	t_collider	*col;

	query = *(ecs_query(ecs, POSITION | VELOCITY | COLLIDER));
	while (query.len--)
	{
		pos = ecs_entity_get(ecs, query.values[query.len], POSITION);
		vel = ecs_entity_get(ecs, query.values[query.len], VELOCITY);
		col = ecs_entity_get(ecs, query.values[query.len], COLLIDER);
		pos->x = fclampf(pos->x + vel->x, app->params.width - col->w - 1, 1);
		pos->y = fclampf(pos->y + vel->y, app->params.height - col->h - 1, 1);
		if (pos->x <= 1 || pos->x + col->w >= app->params.width - 1)
			vel->x = -vel->x;
		if (pos->y <= 1 || pos->y + col->h >= app->params.height - 1)
			vel->y = -vel->y;
		box_draw(app, *pos, col->w, col->h, col->is_colliding ? 0xFFFF0000 : 0xFFFFFFFF);
		col->is_colliding = false;
		grid_insert(grid, query.values[query.len], *pos);
	}
}

static void	__init(t_app *app, t_scene *scene)
{
	uint16_t	i;
	t_env		*env;

	(void) app;
	env = malloc(sizeof(t_env));
	if (!env)
		return ;
	scene->env = env;
	env->grid = grid_create((t_vector){0, 0}, 40);
	env->ecs = ecs_create(3, sizeof(t_vector),
					sizeof(t_vector), sizeof(t_collider));
	srand(time(NULL));
	i = -1;
	while (++i < ECS_ENTITY_CAP - 1)
		box_create(env->ecs,
			 ((float)rand()/(float)(RAND_MAX)) * app->params.width,
			 ((float)rand()/(float)(RAND_MAX)) * app->params.height,
			 cos(rand()/(float)(RAND_MAX) * M_PI * 2),
			 sin(rand()/(float)(RAND_MAX) * M_PI * 2));
}

static void	__update(t_app *app, t_scene *scene)
{
	t_env	*env;

	env = (t_env *)scene->env;
	mlx_clear_window(app->mlx, app->win);
	grid_clear(env->grid);
	box_system(env->ecs, app, env->grid);
	grid_process(env->grid, env->ecs, collisionCallback);
	grid_draw(app, env->grid);
}

static void	__event(t_app *app, t_scene *scene, mlx_event_type t, int e)
{
	(void) scene;
	if (t == MLX_KEYDOWN && e == 41)
		mlx_loop_end(app->mlx);
}

static void	__clear(t_app *app, t_scene *scene)
{
	(void) app;
	if (!scene->env)
		return ;
	grid_destroy(((t_env *)scene->env)->grid);
	ecs_destroy(((t_env *)scene->env)->ecs);
	free(scene->env);
}

int	main(void)
{
	t_scene	scenes[1];

	scenes[0] = (t_scene){NULL, "main", __init, __event, __update, __clear};
	app_autorun((t_win_params){1280, 720, 60, "collisions"}, scenes, 1);
}
