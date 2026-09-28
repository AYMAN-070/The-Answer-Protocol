/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_setup_fd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 09:33:47 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/28 09:41:08 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	setup_fd_set(int server_fd, t_player *clients, fd_set *readfds)
{
	int	i;
	int	max_sd;

	FD_ZERO(readfds);
	FD_SET(0, readfds);
	FD_SET(server_fd, readfds);
	max_sd = server_fd;
	i = 0;
	while (i < 10)
	{
		if (clients[i].is_active == 1)
		{
			FD_SET(clients[i].sd, readfds);
			if (clients[i].sd > max_sd)
				max_sd = clients[i].sd;
		}
		i++;
	}
	return (max_sd);
}

void	handle_server_input(int *running, fd_set *readfds)
{
	char	buf[64];
	int		r;

	if (FD_ISSET(0, readfds))
	{
		r = read(0, buf, 63);
		if (r > 0)
		{
			buf[r] = '\0';
			if (strncmp(buf, "exit", 4) == 0 || strncmp(buf, "quit", 4) == 0)
			{
				server_log("INFO", "sys", "none", "CLosing server ...");
				*running = 0;
			}
		}
	}
}
