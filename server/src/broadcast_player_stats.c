/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast_player_stats.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 15:04:43 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/18 23:45:45 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	broadcast_player_stats(t_player *c)
{
	int		i;
	int		total;
	char	out[64];

	total = 0;
	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active == 1 && c[i].name[0] != '\0')
			total++;
	}
	sprintf(out, "EVT STATS players=%d\n", total);
	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active == 1)
			write(c[i].sd, out, strlen(out));
	}
}
