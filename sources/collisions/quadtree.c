/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadtree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:41:09 by mykle             #+#    #+#             */
/*   Updated: 2024/11/27 13:42:32 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quadtree.h"

t_quadtree_node* createNode(t_vector transform, t_collider collider)
{
	t_quadtree_node* node;
	uint8_t			i;

	node = malloc(sizeof(t_quadtree_node));
	if (__builtin_expect(!node, 0))
		return (NULL);
	node->transform = transform;
	node->collider = collider;
	i = -1;
	while (++i < 4)
		node->children[i] = NULL;
    return (node);
}
