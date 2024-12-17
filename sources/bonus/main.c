/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:30:01 by mrouves           #+#    #+#             */
/*   Updated: 2024/12/18 00:20:44 by mrouves          ###   ########.fr       */
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

static int	__app_start(t_app *app)
{
	t_prog_args			*args;
	static const char	*p_hero[NB_IMGS_HERO] = {
		"resources/hero/idle_f2.png",
		"resources/hero/bullet.png"};
	static const char	*p_env[NB_IMGS_ENV] = {
		"resources/env/tile041.png"};
	static const char	*p_bg[NB_IMGS_BG] = {
		"resources/env/background.png"};

	args = (t_prog_args *)app->params.args;
	if (load_imgs(app->mlx, p_hero, args->imgs_hero, NB_IMGS_HERO) == APP_ERROR
		|| load_imgs(app->mlx, p_env, args->imgs_env, NB_IMGS_ENV) == APP_ERROR
		|| load_imgs(app->mlx, p_bg, args->imgs_bg, NB_IMGS_BG) == APP_ERROR)
		return (APP_ERROR);
	return (0);
}

static int	__app_stop(t_app *app)
{
	t_prog_args	*args;

	args = (t_prog_args *)app->params.args;
	unload_imgs(app->mlx, args->imgs_bg, NB_IMGS_BG);
	unload_imgs(app->mlx, args->imgs_env, NB_IMGS_ENV);
	unload_imgs(app->mlx, args->imgs_hero, NB_IMGS_HERO);
	return (0);
}

int	main(int ac, char **av)
{
	static t_prog_args	args = {0};
	t_parse_error		status;

	args.argc = ac;
	args.argv = av;
	status = PARSE_ERROR_FILECRASH;
	if (ac == 2)
		status = tilemap_parse(&args.tilemap, av[1]);
	if (status == PARSE_OK)
		status = tilemap_check(&args.tilemap);
	if (status == PARSE_OK)
	{
		app_autorun((t_win_params){
			__app_start, __app_stop, &args, "so_long", 800, 600, 60}, NB_SCENES,
			(t_scene){NULL, 0,
			__menu_init, __menu_event, __menu_update, __menu_clear},
			(t_scene){NULL, sizeof(t_game),
			__game_init, __game_event, __game_update, __game_clear});
	}
	else
		print_parse_error(status);
	tilemap_destroy(&args.tilemap);
}
