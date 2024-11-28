/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualization.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:58:46 by mykle             #+#    #+#             */
/*   Updated: 2024/11/28 15:18:22 by mrouves          ###   ########.fr       */
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
    t_aabb		bounds;            // Limites de ce nœud
    uint32_t	items[MAX_ITEMS]; // IDs des objets stockés dans ce nœud
    uint16_t	count;          // Nombre actuel d'objets dans ce nœud
    uint16_t	child;         // Index du premier enfant (dans le tableau global), -1 s'il n'y en a pas
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


// Prototypes
t_qtree *createQuadTree(t_aabb bounds, uint16_t cap);
void freeQuadTree(t_qtree *qt);
int insertItem(t_qtree *qt, uint16_t node_index, uint32_t id, t_aabb box);
void subdivide(t_qtree *qt, uint16_t node_index);
void growQuadTree(t_qtree *qt);
bool contains(t_aabb a, t_aabb b);
bool intersects(t_aabb a, t_aabb b);
void detectCollisions(t_qtree *qt, t_ecs *ecs, uint16_t node_index, void (*f)(t_ecs *, uint32_t, uint32_t));

t_qtree *createQuadTree(t_aabb bounds, uint16_t cap)
{
    t_qtree *qt = malloc(sizeof(t_qtree));
    qt->nodes = calloc(cap, sizeof(t_qtree_node));
    qt->count = 1;
    qt->cap = cap;
    qt->nodes[0] = (t_qtree_node){bounds, {0}, 0, 0};
    return (qt);
}

void freeQuadTree(t_qtree *qt)
{
	if (!qt)
		return ;
    free(qt->nodes);
    free(qt);
}

// Fonction pour agrandir la capacité du QuadTree
void growQuadTree(t_qtree *qt)
{
    qt->cap *= 2;
    qt->nodes = realloc(qt->nodes, qt->cap * sizeof(t_qtree_node));
    if (!qt->nodes) {
        fprintf(stderr, "Erreur : Réallocation échouée !\n");
        exit(EXIT_FAILURE);
    }
}

// Fonction pour subdiviser un nœud
void subdivide(t_qtree *qt, uint16_t node_index) {
    if (qt->count + 4 > qt->cap) {
        growQuadTree(qt);
    }

    t_qtree_node *node = qt->nodes + node_index;
    float x = node->bounds.x;
    float y = node->bounds.y;
    float w = node->bounds.w >> 1;
    float h = node->bounds.h >> 1;

    node->child = qt->count;
    qt->nodes[qt->count++] = (t_qtree_node){(t_aabb){x, y, w, h}, {0}, 0, 0};
    qt->nodes[qt->count++] = (t_qtree_node){(t_aabb){x + w, y, w, h}, {0}, 0, 0};
    qt->nodes[qt->count++] = (t_qtree_node){(t_aabb){x, y + h, w, h}, {0}, 0, 0};
    qt->nodes[qt->count++] = (t_qtree_node){(t_aabb){x + w, y + h, w, h}, {0}, 0, 0};
}

// Vérifie si une boîte `b` est contenue dans une boîte `a`
bool contains(t_aabb a, t_aabb b) {
    return (b.x >= a.x && b.x + b.w <= a.x + a.w &&
            b.y >= a.y && b.y + b.h <= a.y + a.h);
}

// Vérifie si deux boîtes se chevauchent
bool intersects(t_aabb a, t_aabb b) {
    return !(b.x >= a.x + a.w || b.x + b.w <= a.x ||
             b.y >= a.y + a.h || b.y + b.h <= a.y);
}

// Insère un ID avec sa boîte AABB dans le QuadTree
int insertItem(t_qtree *qt, uint16_t node_index, uint32_t id, t_aabb box)
{
    t_qtree_node *node = qt->nodes + node_index;
    if (!contains(node->bounds, box))
        return (0); // L'objet est hors des limites
    // Si le nœud a encore de l'espace
    if (node->count < MAX_ITEMS)
	{
        node->items[node->count++] = id;
        return (1);
    }
    // Si le nœud doit être subdivisé
    if (!node->child)
        subdivide(qt, node_index);
    // Insère dans les enfants
    int childIndex = node->child;
    for (int i = 0; i < 4; i++) {
        if (insertItem(qt, childIndex + i, id, box))
            return 1;
    }
    return 0;
}

void clearQuadTree(t_qtree *qt, t_aabb bounds)
{
	ft_memset(qt->nodes, 0, sizeof(t_qtree_node) * qt->cap);
	qt->count = 1;
	qt->nodes[0] = (t_qtree_node){bounds, {0}, 0, 0};
}

// Fonction pour détecter les paires d'objets en collision
void detectCollisions(t_qtree *qt, t_ecs *ecs, uint16_t node_index, void (*f)(t_ecs *, uint32_t, uint32_t))
{
    t_qtree_node *node = qt->nodes + node_index;

    // Vérifie les collisions entre les objets dans ce nœud
    for (int i = 0; i < node->count; i++) {
        for (int j = i + 1; j < node->count; j++) {
			t_collider	*c1 = ecs_entity_get(ecs, node->items[i], COLLIDER);
			t_collider	*c2 = ecs_entity_get(ecs, node->items[j], COLLIDER);
			t_vector	*p1 = ecs_entity_get(ecs, node->items[i], POSITION);
			t_vector	*p2 = ecs_entity_get(ecs, node->items[j], POSITION);
            if (intersects((t_aabb){p1->x, p1->y, c1->w, c2->h}, (t_aabb){p2->x, p2->y, c1->w, c2->h}))
                f(ecs, node->items[i], node->items[j]);
        }
    }

    // Vérifie les collisions entre les objets de ce nœud et des enfants
    if (node->child) {
        for (int i = 0; i < 4; i++) {
            t_qtree_node *child = qt->nodes + node->child + i;
            for (int j = 0; j < node->count; j++) {
				t_collider	*c1 = ecs_entity_get(ecs, node->items[j], COLLIDER);
				t_vector	*p1 = ecs_entity_get(ecs, node->items[j], POSITION);
                for (int k = 0; k < child->count; k++) {
					t_collider	*c2 = ecs_entity_get(ecs, node->items[k], COLLIDER);
					t_vector	*p2 = ecs_entity_get(ecs, node->items[k], POSITION);
					if (intersects((t_aabb){p1->x, p1->y, c1->w, c2->h}, (t_aabb){p2->x, p2->y, c1->w, c2->h}))
						f(ecs, node->items[j], node->items[k]);
                }
            }
        }
    }
    // Appelle récursivement la fonction pour les enfants
    if (node->child)
	{
        for (int i = 0; i < 4; i++)
            detectCollisions(qt, ecs, node->child + i, f);
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
	ecs_entity_add(ecs, id, COLLIDER, &(t_collider){10, 10, MOVING, 0});
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
		pos->x = fclampf(pos->x + vel->x, app->params.width - (col->w >> 1) - 1, (col->w >> 1) + 1);
		pos->y = fclampf(pos->y + vel->y, app->params.height - (col->h >> 1) - 1, (col->h >> 1) + 1);
		if (pos->x - (col->w >> 1) < (col->w >> 1) + 1 || pos->x + (col->w >> 1) > app->params.width - (col->w >> 1) - 1)
			vel->x = -vel->x;
		if (pos->y - (col->h >> 1) < (col->h >> 1) + 1 || pos->y + (col->h >> 1) > app->params.height - (col->h >> 1) - 1)
			vel->y = -vel->y;
		insertItem(qt, 0, query.values[query.len], (t_aabb){pos->x, pos->y, col->w, col->h});
		box_draw(app, *pos, col->w, col->h, col->is_colliding ? 0xFFFF0000 : 0xFFFFFFFF);
		col->is_colliding = false;
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
	env->qt = createQuadTree((t_aabb){0, 0, app->params.width, app->params.height}, 1024);
	env->ecs = ecs_create(3, sizeof(t_vector),
					sizeof(t_vector), sizeof(t_collider));
	srand(time(NULL));
	i = -1;
	while (++i < ECS_ENTITY_CAP - 1)
		box_create(env->ecs,
			 ((float)rand()/(float)(RAND_MAX)) * app->params.width,
			 ((float)rand()/(float)(RAND_MAX)) * app->params.height,
			 ((float)rand()/(float)(RAND_MAX)) * 10 - 5,
			 ((float)rand()/(float)(RAND_MAX)) * 10 - 5);
}

static void	__update(t_app *app, t_scene *scene)
{
	t_env	*env;

	env = (t_env *)scene->env;
	mlx_clear_window(app->mlx, app->win);
	clearQuadTree(env->qt, (t_aabb){0, 0, app->params.width, app->params.height});
	box_system(env->ecs, app, env->qt);
	detectCollisions(env->qt, env->ecs, 0, collisionCallback);
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
