/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resources.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 21:03:46 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/20 18:11:44 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bonus.h>

static int	load_imgs(void *mlx, const char **paths, t_sprite *s, uint8_t n)
{
	while (n--)
	{
		if (open(paths[n], O_RDONLY) < 0)
			return (APP_ERROR);
		s[n].texture = mlx_png_file_to_image(mlx,
				(char *)paths[n], &s[n].w, &s[n].h);
		if (!s[n].texture)
			return (APP_ERROR);
	}
	return (0);
}

static void	unload_imgs(void *mlx, t_sprite *s, uint8_t n)
{
	while (n--)
		if (s[n].texture)
			mlx_destroy_image(mlx, s[n].texture);
}

int	unload_app_resources(void *mlx, t_prog_args *args)
{
	unload_imgs(mlx, args->imgs_bg, N_IMGS_BG);
	unload_imgs(mlx, args->imgs_env, N_IMGS_ENV);
	unload_imgs(mlx, args->imgs_hero, N_IMGS_HERO);
	unload_imgs(mlx, args->imgs_other, N_IMGS_OTHER);
	return (0);
}

int	load_app_resources(void *mlx, t_prog_args *args)
{
	static const char	*p_hero[N_IMGS_HERO] = {
		"resources/hero/idle_f2.png",
		"resources/hero/idle_f1.png"};
	static const char	*p_env[N_IMGS_ENV] = {
		"resources/env/tile041.png"};
	static const char	*p_bg[N_IMGS_BG] = {
		"resources/env/background.png"};
	static const char	*p_other[N_IMGS_OTHER] = {
		"resources/bullets/tile076.png", "resources/bullets/tile077.png",
		"resources/bullets/tile078.png", "resources/bullets/tile079.png",
		"resources/bullets/tile091.png", "resources/bullets/tile092.png",
		"resources/bullets/tile093.png", "resources/bullets/tile094.png"};

	if (load_imgs(mlx, p_hero, args->imgs_hero, N_IMGS_HERO) == APP_ERROR
		|| load_imgs(mlx, p_env, args->imgs_env, N_IMGS_ENV) == APP_ERROR
		|| load_imgs(mlx, p_other, args->imgs_other, N_IMGS_OTHER) == APP_ERROR
		|| load_imgs(mlx, p_bg, args->imgs_bg, N_IMGS_BG) == APP_ERROR)
		return (APP_ERROR);
	return (0);
}
