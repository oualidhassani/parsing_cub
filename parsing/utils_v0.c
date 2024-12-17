/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_v0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboudiba <aboudiba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:32:50 by mel-rhay          #+#    #+#             */
/*   Updated: 2024/11/14 19:48:48 by aboudiba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/Cub3D.h"

void	error_print(char *str, t_map *map)
{
	printf(RED "Error\n" RST);
	printf(YELLOW "%s\n" RST, str);
	destroy_game(map);
	exit(1);
}

void	map_initializer(t_map *map)
{
	map->no_texture = NULL;
	map->so_texture = NULL;
	map->we_texture = NULL;
	map->ea_texture = NULL;
	map->floor_color = NULL;
	map->ceiling_color = NULL;
	map->map = NULL;
	map->player.dir = 0;
	map->player.pos_x = -1;
	map->player.pos_y = -1;
	map->floor_rgb.r = -1;
	map->floor_rgb.g = -1;
	map->floor_rgb.b = -1;
	map->ceiling_rgb.r = -1;
	map->ceiling_rgb.g = -1;
	map->ceiling_rgb.b = -1;
	map->sprite_h = 0;
}

bool	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (true);
	return (false);
}

char	*ft_substr_plus(char *s)
{
	int		i;
	char	*str;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] && !ft_isspace(s[i]))
		i++;
	while (s[i] && ft_isspace(s[i]))
		i++;
	str = ft_substr(s, i, ft_strlen(s));
	free(s);
	return (str);
}
