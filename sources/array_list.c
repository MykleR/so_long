/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:46:46 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/03 23:32:03 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <array_list.h>

bool	array_list_create(t_array_list *list, uint8_t memsize)
{
	if (__builtin_expect(!list, 0))
		return (false);
	list->len = 0;
	list->cap = ARRAY_LIST_INIT;
	list->mem = memsize;
	list->data = ft_calloc(memsize, list->cap);
	return (list->data != NULL);
}

void	array_list_destroy(t_array_list *list)
{
	if (__builtin_expect(!list, 0))
		return ;
	free(list->data);
	ft_memset(list, 0, sizeof(t_array_list));
}

void	array_list_remove(t_array_list *list, uint16_t index)
{
	if (__builtin_expect(!list || index >= list->len, 0))
		return ;
	list->len--;
	ft_memcpy(list->data + list->len * list->mem,
		list->data + index * list->mem, list->mem);
}

void	array_list_insert(t_array_list *list, void *ptr)
{
	uint32_t	new_size;

	if (__builtin_expect(!list || !ptr || list->len >= UINT16_MAX, 0))
		return ;
	if (__builtin_expect(list->len >= list->cap, 0))
	{
		new_size = list->cap << 1;
		if (new_size >> 1 != list->cap)
			new_size = UINT16_MAX;
		list->data = ft_realloc(list->data, list->cap * list->mem,
				new_size * list->mem);
		list->cap = new_size;
	}
	ft_memcpy(list->data + list->len * list->mem, ptr, list->mem);
	list->len++;
}

void	*array_list_get(t_array_list *list, uint16_t index)
{
	if (__builtin_expect(!list || index >= list->len, 0))
		return (NULL);
	return (list->data + index * list->mem);
}
