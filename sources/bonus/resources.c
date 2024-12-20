/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resources.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 21:03:46 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/20 21:05:01 by mrouves          ###   ########.fr       */
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
	unload_imgs(mlx, args->imgs_env, N_IMGS_ENV);
	unload_imgs(mlx, args->imgs_hero, N_IMGS_HERO);
	unload_imgs(mlx, args->imgs_other, N_IMGS_OTHER);
	return (0);
}

int	load_app_resources(void *mlx, t_prog_args *args)
{
	static const char	*p_hero[N_IMGS_HERO] = {
		"resources/hero/idle_f2.png", "resources/hero/idle_f1.png"};
	static const char	*p_env[N_IMGS_ENV] = {
		"resources/env/background.png", "resources/env/wall_2.png",
		"resources/env/coin.png", "resources/env/exit_f1.png"};
	static const char	*p_other[N_IMGS_OTHER] = {
		"resources/effects/pbullet_f1.png", "resources/effects/pbullet_f2.png",
		"resources/effects/pbullet_f3.png", "resources/effects/pbullet_f4.png",
		"resources/effects/shoot_f1.png", "resources/effects/shoot_f2.png",
		"resources/effects/shoot_f3.png", "resources/effects/shoot_f4.png"};

	if (load_imgs(mlx, p_hero, args->imgs_hero, N_IMGS_HERO) == APP_ERROR
		|| load_imgs(mlx, p_other, args->imgs_other, N_IMGS_OTHER) == APP_ERROR
		|| load_imgs(mlx, p_env, args->imgs_env, N_IMGS_ENV) == APP_ERROR)
		return (APP_ERROR);
	return (0);
}
