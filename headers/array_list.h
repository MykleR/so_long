/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:39:24 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 21:53:40 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_LIST_H
# define ARRAY_LIST_H

# include "libft.h"

# define ARRAY_LIST_INIT 32

typedef struct	s_array_list
{
	void		*data;
	uint16_t	len;
	uint16_t	cap;
	uint8_t		mem;
}	t_array_list;

bool	array_list_create(t_array_list *list, uint8_t memsize);
void	array_list_destroy(t_array_list *list);
void	array_list_remove(t_array_list *list, uint16_t index);
void	array_list_insert(t_array_list *list, void *ptr);
void	*array_list_get(t_array_list *list, uint16_t index);

#endif
