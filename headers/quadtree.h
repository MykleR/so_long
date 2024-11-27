/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadtree.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mykle <mykle@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:34:17 by mykle             #+#    #+#             */
/*   Updated: 2024/11/27 15:40:24 by mykle            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "uint_list.h"

typedef struct s_quadt_node {
	uint32_t	first_child;
	uint32_t	count;
} t_quadt_node;

t_quadt_node	*quadt_tree_create(t_ecs_ulist *xsorted, t_ecs_ulist *ysorted);
void			quadt_destroy(t_quadt_node *root);
