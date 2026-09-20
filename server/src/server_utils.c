/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 09:44:01 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 16:07:27 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	exit_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	init_clients(t_player *clients)
{
	int	i;

	i = 0;
	while (i < 10)
	{
		clients[i].sd = 0;
		clients[i].is_active = 0;
		memset(clients[i].name, 0, 32);
		clients[i].cmd_len = 0;
		memset(clients[i].cmd_buffer, 0, 1024);
		i++;
	}
}

void	broadcast_room(t_player *c, char *room, char *msg, int exclude_sd)
{
	int	i;

	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active && c[i].sd != exclude_sd
			&& strcmp(c[i].location, room) == 0)
			write(c[i].sd, msg, strlen(msg));
	}
}

void	broadcast_group(t_player *c, int group_id, char *msg, int exclude_sd)
{
	int	i;

	if (group_id == 0)
		return ;
	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active && c[i].sd != exclude_sd
			&& c[i].group_id == group_id)
			write(c[i].sd, msg, strlen(msg));
	}
}

int	init_server(void)
{
	int					server_fd;
	int					opt;
	struct sockaddr_in	address;

	opt = 1;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		exit_error("Socket error");
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(4242);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		exit_error("Bind error");
	if (listen(server_fd, 3) < 0)
		exit_error("Listen error");
	return (server_fd);
}
