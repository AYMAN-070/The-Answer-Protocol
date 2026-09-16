/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Process_buffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 16:09:15 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/16 16:09:30 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	process_buffer(t_player *p, t_player *c, char *buf, char *wld)
{
	char	*start;
	int		i;

	start = buf;
	i = -1;
	while (buf[++i] != '\0')
	{
		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			if (start[0] != '\0')
				process_command(p, c, start, wld);
			start = buf + i + 1;
		}
	}
}