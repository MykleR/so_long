/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resources.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 21:03:46 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/22 00:58:00 by mykle            ###   ########.fr       */
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

static int	load_effects(void *mlx, t_prog_args *args)
{
	static const char	*p_fx[N_IMGS_FX] = {"resources/effects/pbullet_f1.png",
		"resources/effects/pbullet_f2.png", "resources/effects/pbullet_f3.png",
		"resources/effects/pbullet_f4.png", "resources/effects/shoot_f1.png",
		"resources/effects/shoot_f2.png", "resources/effects/shoot_f3.png",
		"resources/effects/shoot_f4.png", "resources/effects/ebullet_f1.png",
		"resources/effects/ebullet_f2.png", "resources/effects/ebullet_f3.png",
		"resources/effects/ebullet_f4.png", "resources/effects/pop_f1.png",
		"resources/effects/pop_f2.png", "resources/effects/pop_f3.png",
		"resources/effects/explo_f1.png", "resources/effects/explo_f2.png",
		"resources/effects/explo_f3.png", "resources/effects/explo_f4.png"};

	return (load_imgs(mlx, p_fx, args->imgs_fx, N_IMGS_FX));
}

int	unload_app_resources(void *mlx, t_prog_args *args)
{
	unload_imgs(mlx, args->imgs_env, N_IMGS_ENV);
	unload_imgs(mlx, args->imgs_hero, N_IMGS_HERO);
	unload_imgs(mlx, args->imgs_fx, N_IMGS_FX);
	unload_imgs(mlx, args->imgs_hud, N_IMGS_HUD);
	return (0);
}

int	load_app_resources(void *mlx, t_prog_args *args)
{
	static const char	*p_hero[N_IMGS_HERO] = {"resources/hero/idle_f2.png",
		"resources/hero/enemy_f1.png", "resources/hero/enemy_f2.png",
		"resources/hero/enemy_f3.png", "resources/hero/enemy_f4.png",
		"resources/hero/death_f1.png", "resources/hero/death_f1.png",
		"resources/hero/death_f2.png", "resources/hero/death_f2.png",
		"resources/hero/death_f3.png", "resources/hero/death_f4.png",
		"resources/hero/death_f5.png", "resources/hero/death_f6.png"};
	static const char	*p_env[N_IMGS_ENV] = {"resources/env/background.png",
		"resources/env/wall_2.png", "resources/env/item_f1.png",
		"resources/env/item_f2.png", "resources/env/item_f3.png",
		"resources/env/item_f4.png", "resources/env/exit_f1.png",
		"resources/env/exit_f2.png", "resources/env/exit_f3.png",
		"resources/env/exit_f4.png"};
	static const char	*p_hud[N_IMGS_HUD] = {"resources/hud/hp_f5.png",
		"resources/hud/hp_f4.png", "resources/hud/hp_f3.png",
		"resources/hud/hp_f2.png", "resources/hud/hp_f1.png"};

	if (load_effects(mlx, args) == APP_ERROR
		|| load_imgs(mlx, p_hud, args->imgs_hud, N_IMGS_HUD) == APP_ERROR
		|| load_imgs(mlx, p_hero, args->imgs_hero, N_IMGS_HERO) == APP_ERROR
		|| load_imgs(mlx, p_env, args->imgs_env, N_IMGS_ENV) == APP_ERROR)
		return (APP_ERROR);
	return (0);
}
