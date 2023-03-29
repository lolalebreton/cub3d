/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-bret <lle-bret@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:52:50 by lle-bret          #+#    #+#             */
/*   Updated: 2023/03/29 17:16:25 by lle-bret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	followRay(t_data *data, t_raycaster *rc)
{
	if (rc->sideDist.x < rc->sideDist.y)
	{
		rc->hitPoint.x = (double) rc->map.x;
		rc->hitPoint.y = ft_doubleAbs(data->player.y - sqrt(pow(rc->sideDist.x, 2)
			- pow(data->player.x - rc->map.x, 2)));
		rc->sideDist.x += rc->deltaDist.x;
		rc->map.x += rc->step.x;
		rc->side = 0;
	}
	else
	{
		rc->hitPoint.y = (double) rc->map.y;
		rc->hitPoint.x = ft_doubleAbs(data->player.x - sqrt(pow(rc->sideDist.y, 2)
			- pow(data->player.y - rc->map.y, 2)));
		rc->sideDist.y += rc->deltaDist.y;
		rc->map.y += rc->step.y;
		rc->side = 1;
	}
	if (data->map->content[rc->map.y][rc->map.x] == '1')
		rc->hit = 1;
}

void	draw_verLine(t_data *data, t_raycaster *rc, int screenX)
{
	rc->cameraX = 2 * screenX / (double) WINDOW_WIDTH - 1;
	rc->rayDir = vect_sum(rc->dir, scalar_mult(rc->plane, rc->cameraX));
	rc->map.x = (int) rc->pos.x;
	rc->map.y = (int) rc->pos.y;
	rc->deltaDist = set_vect(ft_doubleAbs(1 / rc->rayDir.x),
		ft_doubleAbs(1 / rc->rayDir.y));
	rc->step = set_step(rc->rayDir);
	rc->sideDist = set_vect(rc->step.x * (rc->map.x - rc->pos.x
		+ (rc->step.x >= 0)) * rc->deltaDist.x, rc->step.y
		* (rc->map.y - rc->pos.y + (rc->step.y >= 0)) * rc->deltaDist.y);
	rc->hit = 0;
	while (rc->hit == 0)
		followRay(data, rc);
	rc->perpWallDist = (rc->sideDist.x - rc->deltaDist.x) * (rc->side == 0)
		+ (rc->sideDist.y - rc->deltaDist.y) * (rc->side != 0);
	rc->lineHeight = (int) (WINDOW_HEIGHT / rc->perpWallDist);
	rc->drawStart = ft_max(0, -rc->lineHeight / 2 + WINDOW_HEIGHT / 2);
	rc->drawEnd = ft_min(WINDOW_HEIGHT - 1, rc->lineHeight / 2
		+ WINDOW_HEIGHT / 2);
	img_verLine_put(data, screenX, *rc);
}

void	raycaster(t_data *data)
{
	t_raycaster	rc;
	double		time;
	double		oldTime;
	int			screenX;

	(void) time;
	(void) oldTime;
	draw_background(data);
	rc.pos = set_vect(data->player.x, data->player.y);
	rc.dir = set_vect(data->direction.x, data->direction.y);
	rc.plane = set_vect(-0.66 * data->direction.y, -0.66 * data->direction.x);
	time = 0;
	oldTime = 0;
	screenX = 0;
	while (screenX < WINDOW_WIDTH)
	{
		draw_verLine(data, &rc, screenX);
		++screenX;
    }
	mlx_put_image_to_window(data->mlx, data->win, data->screen->img, 0, 0);
}
