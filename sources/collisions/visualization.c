/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualization.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:58:46 by mykle             #+#    #+#             */
/*   Updated: 2024/11/28 17:15:52 by mrouves          ###   ########.fr       */
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

#define MAX_ITEMS 4

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
    uint16_t	count;			// Nombre actuel d'objets dans ce nœud
    uint16_t	child_index;	// Index du premier enfant (dans le tableau global), -1 s'il n'y en a pas
} t_qtree_node;

typedef struct {
    t_qtree_node	*nodes; // Tableau contenant tous les nœuds
    uint16_t		count;       // Nombre actuel de nœuds
    uint16_t		cap;        // Capacité actuelle du tableau de nœuds
} t_qtree;


typedef struct	s_env
{
	t_qtree *qt;
	t_ecs	*ecs;
}	t_env;


t_qtree *createQuadTree(t_aabb bounds, uint16_t cap)
{
	(void) bounds;
	(void) cap;
	return (NULL);
}

void freeQuadTree(t_qtree *qt)
{
	(void)qt;
}

// Fonction pour agrandir la capacité du QuadTree
void growQuadTree(t_qtree *qt)
{
	(void)qt;
}

// Fonction pour subdiviser un nœud
void subdivide(t_qtree *qt, uint16_t node_index)
{
	(void)qt; (void) node_index;
}

// Vérifie si une boîte `b` est contenue dans une boîte `a`
bool contains(t_aabb a, t_aabb b) {
    return (b.x > a.x && b.x < a.x + a.w &&
            b.y > a.y && b.y < a.y + a.h);
}

// Vérifie si deux boîtes se chevauchent
bool intersects(t_aabb a, t_aabb b) {
    return !(b.x > a.x + a.w || b.x + b.w < a.x ||
             b.y > a.y + a.h || b.y + b.h < a.y);
}

int insertItem(t_qtree *qt, uint16_t node_index, uint32_t id, t_aabb box)
{
	(void)qt; (void) node_index; (void)id; (void)box;
	return (0);
}

void clearQuadTree(t_qtree *qt, t_aabb bounds)
{
	(void)qt;(void)bounds;
}


void detectCollisions(t_qtree *qt, t_ecs *ecs, uint16_t node_index, void (*f)(t_ecs *, uint32_t, uint32_t))
{
	(void) f; (void) node_index; (void) ecs; (void) qt;
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

int	wich_quadrent(t_vector pos, t_vector center)
{
	if (pos.y < center.y)
		return (pos.x >= center.x);
	return ((pos.x >= center.x) + 2);
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

void	bound_draw(t_app *app, t_aabb bound, uint32_t color)
{
	box_draw(app, (t_vector){bound.x, bound.y}, bound.w, bound.h, color);
}

void	box_system(t_ecs *ecs, t_app *app, t_qtree *qt)
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
		insertItem(qt, 0, query.values[query.len], (t_aabb){round(pos->x), round(pos->y), col->w, col->h});
		box_draw(app, *pos, col->w, col->h, col->is_colliding ? 0xFFFF0000 : 0xFFFFFFFF);
		col->is_colliding = false;
	}
}

void drawQuadTree(t_app *app, t_qtree *qt, uint16_t index)
{
	(void)app;
	(void)qt;
	(void)index;
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
	env->qt = createQuadTree((t_aabb){0, 0, app->params.width, app->params.height}, 1024);
	env->ecs = ecs_create(3, sizeof(t_vector),
					sizeof(t_vector), sizeof(t_collider));
	srand(time(NULL));
	i = -1;
	while (++i < ECS_ENTITY_CAP - 1)
		box_create(env->ecs,
			 ((float)rand()/(float)(RAND_MAX)) * app->params.width,
			 ((float)rand()/(float)(RAND_MAX)) * app->params.height,
			 cos(rand()/(float)(RAND_MAX) * 2 * M_PI - M_PI) / 2,
			 sin(rand()/(float)(RAND_MAX) * 2 * M_PI - M_PI) / 2);
}

static void	__update(t_app *app, t_scene *scene)
{
	t_env	*env;

	env = (t_env *)scene->env;
	mlx_clear_window(app->mlx, app->win);
	clearQuadTree(env->qt, (t_aabb){0, 0, app->params.width, app->params.height});
	box_system(env->ecs, app, env->qt);
	detectCollisions(env->qt, env->ecs, 0, collisionCallback);
	drawQuadTree(app, env->qt, 0);
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
	freeQuadTree(((t_env *)scene->env)->qt);
	ecs_destroy(((t_env *)scene->env)->ecs);
	free(scene->env);
}

int	main(void)
{
	t_scene	scenes[1];

	scenes[0] = (t_scene){NULL, "main", __init, __event, __update, __clear};
	app_autorun((t_win_params){1280, 720, 60, "collisions"}, scenes, 1);
}
